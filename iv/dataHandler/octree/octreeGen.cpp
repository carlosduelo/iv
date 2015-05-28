/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeGen.h>

#include <iv/common/init.h>
#include <iv/common/mortonCodeUtil_CPU.h>

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>
#include <iv/dataHandler/octree/dataWarehouse.h>

#include <algorithm>
#include <thread>
#include <chrono>

#include <omp.h>

#include <boost/progress.hpp>

namespace
{

inline bool checkIsosurface( const float xyz,
                             const float xyz1,
                             const float xy1z,
                             const float xy1z1,
                             const float x1yz,
                             const float x1yz1,
                             const float x1y1z,
                             const float x1y1z1,
                             const float iso )
{
    const bool sign = ( xyz - iso ) < 0;

    if( ( ( xyz1 - iso ) < 0 ) != sign )
       return true;
    if( ( ( xy1z - iso ) < 0 ) != sign )
       return true;
    if( ( ( xy1z1 - iso ) < 0 ) != sign )
       return true;
    if( ( ( x1yz - iso ) < 0 ) != sign )
       return true;
    if( ( ( x1yz1 - iso ) < 0 ) != sign )
       return true;
    if( ( ( x1y1z - iso ) < 0 ) != sign )
       return true;
    if( ( ( x1y1z1 - iso ) < 0 ) != sign )
        return true;

    return false;
}

class PlaneReader
{
public:
    bool init()
    {
        const iv::Global& global = iv::IV::getGlobal();
        _file = iv::DataHandler::FactoryFileHandler::CreateFileHandler(
                                                        global.getFileType(),
                                                        global.getFileArgs() );
        if( !_file )
            return false;
        return true;
    }

    const iv::vec3int32_t& getDimension()
    {
        return _file->getRealDimension();
    }

    void readPlane( float* plane, const uint32_t x )
    {
        _thread  = std::thread( &PlaneReader::_readPlane, this, plane, x );
    }

    void wait( )
    {
        if( _thread.joinable() )
            _thread.join();
    }

private:
    void _readPlane( float* plane, const uint32_t x )
    {
        const iv::vec3int32_t& dimension = _file->getRealDimension();
        _file->read( plane,
                     iv::vec3int32_t( x, 0, 0 ),
                     iv::vec3int32_t( x + 1, dimension.y(), dimension.z() ) );
    }

    iv::DataHandler::FileHandlerPtr _file;
    std::thread                     _thread;
};

class OctreeComplete
{
public:
    OctreeComplete( const uint32_t levels )
        : _levels( levels )
    {
        _cubes.reset( new std::vector< iv::index_node_t >[ levels ] );
    }

    void addCube( const iv::index_node_t id, const uint32_t level )
    {
        const uint32_t size = _cubes[ level ].size();
        // Firts
        if( size == 0 )
        {
            _cubes[ level ].push_back( id );
            _cubes[ level ].push_back( id );
        }
        else if( _cubes[ level ].back() == (id - (iv::index_node_t)1 ) )
        {
            _cubes[ level ][ size - 1 ] = id;
        }
        else if( _cubes[level].back() == id )
        {
            //std::cout<<"repetido in level "<<level<<" "<< id <<std::endl;
        }
        else if( _cubes[level].back() > id )
        {
            std::cout << "=======>   ERROR: insert index in order "
                      << id << " (in level " << level<< ") last inserted "
                      << _cubes[ level ].back() << std::endl;
            throw;
        }
        else
        {
            _cubes[level].push_back(id);
            _cubes[level].push_back(id);
        }
    }

    void addCubes( const std::vector< iv::index_node_t >& cubes, const uint32_t n )
    {
        iv::index_node_t maxid = 0;
        for( uint32_t i = 0; i < n * 2; i+= 2 )
        {
            const iv::index_node_t minID = cubes[ i ];
            const iv::index_node_t maxID = cubes[ i + 1 ];
            for( auto idC = minID; idC <= maxID; idC++ )
            {
                iv::index_node_t id = idC;
                if( id < maxid )
                {
                    std::cout << "======" << minID << " " << maxID << std::endl;
                    return;
                }
                maxid = id;
                for( int32_t l = _levels - 1; l >= 0; l--)
                {
                    id >>= 3;
                    addCube( id, l );
                }
            }
        }
    }

    const uint32_t                                          _levels;
    std::unique_ptr< std::vector< iv::index_node_t >[] >    _cubes;
};

}

namespace iv
{

namespace DataHandler
{

bool OctreeGen::compute( )
{
    const Global& global = IV::getGlobal();

    if( global.getOctreeFile() == "" )
    {
        std::cerr << "No octree file specified" << std::endl;
        return false;
    }

    if( global.getIsosurfaces().size() == 0 )
    {
        std::cerr << "Not isosurfaces provided" << std::endl;
        return false;
    }

    if( global.getFileType() != IV_FILE_TYPE_HDF5 )
    {
        std::cerr << "Provide hdf5 file" << std::endl;
        return false;
    }

    // Init DataWarehouse
    PlaneReader planeReader;
    planeReader.init();

    const iv::vec3int32_t& dimension = planeReader.getDimension();

    DataWarehousePtr dataWarehouse( new DataWarehouse( dimension ) );
    if( !dataWarehouse->start() )
    {
        std::cout << "Error init DataWarehouse" << std::endl;
        return false;
    }

    float* plane0 = new float[ dimension.y() * dimension.z() ];
    float* plane1 = new float[ dimension.y() * dimension.z() ];
    float* plane2 = new float[ dimension.y() * dimension.z() ];
    planeReader.readPlane( plane1, 0 );
    planeReader.wait();
    planeReader.readPlane( plane2, 1 );

    std::cout << "Reading volume and computing isosurface" << std::endl;
    auto startC = std::chrono::high_resolution_clock::now();
    boost::progress_display show_progress( dimension.x() - 1 );
    int32_t plane = 0;
    int32_t timeComputing = 0;
    int32_t timeWaiting = 0;
    do
    {
        // Reading next plane
        auto startCR = std::chrono::high_resolution_clock::now();
        planeReader.wait();
        auto endCR = std::chrono::high_resolution_clock::now();
        timeWaiting += std::chrono::duration_cast< std::chrono::milliseconds>( endCR - startCR ).count();
        float* aux = plane0;
        plane0 = plane1;
        plane1 = plane2;
        plane2 = aux;
        planeReader.readPlane( plane2, plane + 2 );

        auto startCC = std::chrono::high_resolution_clock::now();
        #pragma omp parallel for
        for( int32_t y = 0; y < dimension.y() - 1; y++ )
        {
            for( int32_t z = 0; z < dimension.z() - 1; z++ )
            {
                for( auto iso = global.getIsosurfaces().begin();
                          iso != global.getIsosurfaces().end();
                          iso++ )
                {
                    if( checkIsosurface( plane0[ y * dimension.z() + z ],
                                         plane0[ y * dimension.z() + ( z + 1 ) ],
                                         plane0[ ( y + 1 ) * dimension.z() + z ],
                                         plane0[ ( y + 1 ) * dimension.z() + ( z + 1 ) ],
                                         plane1[ y * dimension.z() + z ],
                                         plane1[ y * dimension.z() + ( z + 1 ) ],
                                         plane1[ ( y + 1 )* dimension.z() + z ],
                                         plane1[ ( y + 1 )* dimension.z() + ( z + 1 ) ],
                                         *iso ) )
                    {
                        dataWarehouse->pushCube( plane, y, z );
                        break;
                    }
                }
            }
        }
        auto endCC = std::chrono::high_resolution_clock::now();
        timeComputing += std::chrono::duration_cast< std::chrono::milliseconds>( endCC - startCC ).count();


        plane++;
        show_progress += 1;
    } while( plane < dimension.x() - 1 );
    planeReader.wait();
    auto endC = std::chrono::high_resolution_clock::now();

    auto timeC = std::chrono::duration_cast< std::chrono::milliseconds>( endC - startC );
    std::cout << timeC.count() << std::endl;
    std::cout << timeComputing << std::endl;
    std::cout << timeWaiting << std::endl;

    delete[] plane0;
    delete[] plane1;
    delete[] plane2;

    dataWarehouse->write();

#if 0
    const uint32_t numRanges = dataWarehouse->getRanges().size() / 2;
    const uint32_t maxHeight = dataWarehouse->getMaxHeight();

    // Write to file
    std::ofstream file( global.getOctreeFile().c_str(), std::ofstream::binary );

    level_t nLevels = global.getnLevels();
    level_t level = global.getOctreeLevel();
    const uint32_t numIsos = global.getIsosurfaces().size() ;
    file.write( (char*)&nLevels, sizeof( nLevels ) );
    file.write( (char*)&level, sizeof( level ) );
    file.write( (char*)&maxHeight, sizeof( maxHeight ) );
    file.write( (char*)&numIsos, sizeof( numIsos ) );
    for( auto i = global.getIsosurfaces().begin();
              i != global.getIsosurfaces().end();
              ++i )
    {
        const float iso = *i;
        file.write( (char*)&iso, sizeof( iso ) );
    }

    // Write number of number of last level ranges
    file.write( (char*)&numRanges, sizeof( numRanges ) );

    OctreeComplete OC( nLevels );

    std::cout << "Building the octree" << std::endl;
    OC.addCubes( dataWarehouse->getRanges(), numRanges );

    std::cout << "Writing the octree" << std::endl;
    file.write( (char*)dataWarehouse->getRanges().data(), 2 * numRanges * sizeof( index_node_t ) );

    for( int32_t l = nLevels - 1; l>=0; l-- )
    {
       const std::vector< index_node_t >& v = OC._cubes[l];
       assert( OC._cubes[l].size() % 2 == 0 );
       const uint32_t nR = OC._cubes[l].size() / 2;
       // Write number of number ranges
       file.write( (char*)&nR, sizeof( nR ) );
       file.write( (char*)v.data(), 2 * nR * sizeof( index_node_t ) );
    }

    file.close();
#endif

    return true;
}

}

}
