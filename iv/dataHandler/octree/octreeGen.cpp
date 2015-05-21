/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeGen.h>

#include <iv/common/init.h>
#include <iv/common/mortonCodeUtil_CPU.h>

#include <iv/dataHandler/octree/octreeConstructor.h>

#include <algorithm>

namespace
{

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

void OctreeGen::printStats() const
{
    std::cout << "Octree Construction Stats:" << std::endl;
    std::cout << "\tData read: " << _stats.getBytesRead() / 1024.0 / 1024.0
              << " MB" << std::endl;
    std::cout << "\tCubes computed: " << _stats.getCubesComputed() << std::endl;
    std::cout << "\tTime Reading: " << _stats.getReadingTime().count() /
                                       1000.0
              << " sec" << std::endl;
    std::cout << "\tBandwidth: "
              << ( _stats.getBytesRead() / 1024.0 / 1024.0 ) /
                 ( _stats.getReadingTime().count() / 1000.0 )
              << " MB/s" << std::endl;
    std::cout << "\tTime Computing: " << _stats.getComputingTime().count() /
                                         1000.0
              << " sec" << std::endl;
    std::cout << "\tTime Sorting: " << _stats.getSortingTime().count() /
                                       1000.0
              << " sec" << std::endl;
}

bool OctreeGen::compute( std::vector< index_node_t >& cubes )
{
    if( cubes.size() == 0 )
        return false;

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

    // Sort cubes
    std::sort( cubes.begin(), cubes.end() );

    std::vector< OctreeConstructorPtr > constructors;
    constructors.resize( cubes.size() );
    for( uint32_t i = 0; i < cubes.size(); i++ )
        constructors[i].reset( new OctreeConstructor( _constructorLevel,
                                                      cubes[i] ) );

    bool wasFine = true;
    for( auto c = constructors.begin(); c != constructors.end(); ++c )
        wasFine &= (*c)->compute();

    if( !wasFine )
        return false;

    // Collect octreeStatistics
    for( auto c = constructors.begin(); c != constructors.end(); ++c )
    {
        _stats.incReadBytes( (*c)->getStats().getBytesRead() );
        _stats.incCube( (*c)->getStats().getCubesComputed() );
        _stats.incReadingTime( (*c)->getStats().getReadingTime() );
        _stats.incComputingTime( (*c)->getStats().getComputingTime() );
        _stats.incSortingTime( (*c)->getStats().getSortingTime() );
    }

    uint32_t maxRanges = 0;
    uint32_t numRanges = 0;
    uint32_t maxHeight = 0;
    for( auto c = constructors.begin(); c != constructors.end(); ++c )
    {
        maxRanges = maxRanges > (*c)->getData()->getNumRanges()
                        ? maxRanges
                        : (*c)->getData()->getNumRanges();
        numRanges += (*c)->getData()->getNumRanges();
        maxHeight = (*c)->getData()->getMaxHeight() > maxHeight
                        ? (*c)->getData()->getMaxHeight()
                        : maxHeight;
    }

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
    std::vector< index_node_t > data;
    data.reserve( maxRanges * 2 );
    for( auto c = constructors.begin(); c != constructors.end(); ++c )
    {
        const uint32_t numR = (*c)->getData()->getNumRanges();
        const std::string file_name = (*c)->getData()->getFileData();
        std::ifstream fileB;
        fileB.open( file_name.c_str(), std::ifstream::binary );
        fileB.read( (char*)data.data(), 2 * numR * sizeof( index_node_t ) );
        fileB.close();

        OC.addCubes( data, numR );
        file.write( (char*)data.data(), 2 * numR * sizeof( index_node_t ) );
    }

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

    return true;
}

}

}
