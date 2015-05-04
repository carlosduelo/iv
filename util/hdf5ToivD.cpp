/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>
#include <iv/common/global.h>
#include <iv/common/mortonCodeUtil_CPU.h>

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>

#include <boost/progress.hpp>

#include <fstream>
#include <chrono>

namespace iv
{

namespace util
{

void createivD( )
{
    const Global& global = IV::getGlobal();

    level_t level = global.getCubeLevel();
    int32_t cubeInc = global.getCubeInc();
    if( global.getFileType() != IV_FILE_TYPE_HDF5 )
    {
        std::cerr << "Provide hdf5 file" << std::endl;
        return;
    }

    if( global.getOutputFile() == "" )
    {
        std::cerr << "Provide output file" << std::endl;
        return;
    }
    std::string file_path( global.getOutputFile() );

    // Open File
    DataHandler::FileHandlerPtr file =
            DataHandler::FactoryFileHandler::CreateFileHandler( global.getFileType(),
                                                                global.getFileArgs() );
    if( !file )
    {
        std::cerr << "Error opening file " << std::endl;
        return;
    }

    const vec3int32_t& dimension = file->getRealDimension();
    level_t nLevels = global.getnLevels();

    // Write to file
    std::ofstream fileO( file_path.c_str(), std::ofstream::out | std::ofstream::binary );
    fileO.write( (char*)&nLevels, sizeof( nLevels ) );
    fileO.write( (char*)&level, sizeof( level ) );
    fileO.write( (char*)&cubeInc, sizeof( cubeInc ) );
    fileO.write( (char*)&dimension[0], sizeof( dimension.x() ) );
    fileO.write( (char*)&dimension[1], sizeof( dimension.y() ) );
    fileO.write( (char*)&dimension[2], sizeof( dimension.z() ) );
    fileO.write( (char*)file->getxGrid(), dimension.x() * sizeof( float ) );
    fileO.write( (char*)file->getyGrid(), dimension.y() * sizeof( float ) );
    fileO.write( (char*)file->getzGrid(), dimension.z() * sizeof( float ) );

    const index_node_t idStart  = 1 << 3 * level;
    const index_node_t idFinish = ( 2 << 3 * level ) - 1;

    // Reserve offset vector
    std::vector< int32_t > offsets;
    offsets.reserve( idFinish - idStart + 1 );

    // Create offsets
    int32_t offset = 0;
    for( index_node_t id = idStart; id <= idFinish; id++ )
    {
        vec3int32_t coordCubeStart = getMinBoxIndex2( id, level, nLevels );
        if( coordCubeStart.x() < dimension.x()  &&
            coordCubeStart.y() < dimension.y()  &&
            coordCubeStart.z() < dimension.z() )
        {
            offsets[ id - idStart ] = offset;
            offset += 1;
        }
        else
        {
            offsets[ id - idStart ] = -1; // mark as not available
        }
    }
    // Write Offsets
    fileO.write( (char*)offsets.data(),
                 ( idFinish - idStart + 1 ) * sizeof( int32_t ) );

    const std::streampos startData =    sizeof( nLevels ) +
                                        sizeof( level ) +
                                        sizeof( cubeInc ) +
                                        sizeof( dimension.x() ) +
                                        sizeof( dimension.y() ) +
                                        sizeof( dimension.z() ) +
                                        dimension.x() * sizeof( float ) +
                                        dimension.y() * sizeof( float ) +
                                        dimension.z() * sizeof( float ) +
                                        ( idFinish - idStart + 1 ) * sizeof( int32_t );

    const uint32_t dim = exp2f( nLevels - level );
    const int32_t limitX = dimension.x() + ( dimension.x() % dim > 0
                                            ? dim - ( dimension.x() % dim  )
                                            : 0);
    const int32_t limitY = dimension.y() + ( dimension.y() % dim > 0
                                            ? dim - ( dimension.y() % dim  )
                                            : 0);
    const int32_t limitZ = dimension.z() + ( dimension.z() % dim > 0
                                            ? dim - ( dimension.z() % dim  )
                                            : 0);

    const vec3int32_t dimPlanes( dim + 2 * cubeInc,
                                 limitY + 2 * cubeInc,
                                 limitZ + 2 * cubeInc );
    const vec3int32_t dimCube( dim + 2 * cubeInc,
                               dim + 2 * cubeInc,
                               dim + 2 * cubeInc );

    const uint32_t cubeSize = powf( dim + 2 * cubeInc, 3 );
    const uint32_t bufferPlanesSize = dimPlanes.x() * // Planes
                                      dimPlanes.y() *
                                      dimPlanes.z();
    // Allocating memory
    std::unique_ptr< float[] > planes( new float[ bufferPlanesSize ] );
    std::unique_ptr< float[] > cube( new float[ cubeSize ] );

    boost::progress_display show_progress( limitX );
    std::chrono::seconds rTime( 0 );
    std::chrono::seconds wTime( 0 );
    for( int32_t x = 0; x < limitX; x += dim )
    {
        const vec3int32_t startPlanes( x - cubeInc, -cubeInc, -cubeInc );
        const vec3int32_t endPlanes = startPlanes + dimPlanes;
        auto startR = std::chrono::high_resolution_clock::now();
        file->read( planes.get(), startPlanes, endPlanes );
        auto endR = std::chrono::high_resolution_clock::now();
        rTime += std::chrono::duration_cast< std::chrono::seconds >( endR - startR );

        for( int32_t y = 0; y < limitY; y+= dim )
        {
            for( int32_t z = 0; z < limitZ; z+= dim )
            {
                const vec3int32_t  coordCube( x, y, z );
                const index_node_t id = coordinateToIndex( coordCube, level, nLevels );
                if( offsets[ id - idStart ] >= 0 )
                {
                    const vec3int32_t coordPlanes = coordCube - startPlanes;
                    // Read Cube
                    uint32_t offsetCube = 0;
                    for( int32_t xC = 0; xC < dimCube.x(); xC++ )
                    {
                        for( int32_t yC = 0; yC < dimCube.y(); yC++ )
                        {
                            const uint32_t offsetPlanes =
                                        ( coordPlanes.z() - cubeInc ) +
                                        ( coordPlanes.y() - cubeInc + yC ) * ( dimPlanes.z() ) +
                                        ( coordPlanes.x() - cubeInc + xC ) * ( dimPlanes.z() * dimPlanes.y() );
                            memcpy( (void*) ( cube.get() + offsetCube ),
                                    (void*) ( planes.get() + offsetPlanes ),
                                    dimCube.z() * sizeof( float ) );

                            offsetCube += dimCube.z();
                        }
                    }
                    // Write Cube
                    auto startW = std::chrono::high_resolution_clock::now();
                    fileO.seekp( startData +
                                    (std::streampos)offsets[ id - idStart ] *
                                    (std::streampos)cubeSize *
                                    (std::streampos)sizeof( float ) );
                                 //std::ios_base::beg );
                    fileO.write( (char*)cube.get(), cubeSize * sizeof( float ) );
                    auto endW = std::chrono::high_resolution_clock::now();
                    wTime += std::chrono::duration_cast< std::chrono::seconds >( endW - startW );
                }
            }
        }
        show_progress += dim;
    }

    fileO.close();

    std::cout << "Reading time " << rTime.count() << " seconds" << std::endl;
    std::cout << "Writing time " << wTime.count() << " seconds" << std::endl;
}

}

}

int main( int argc, char ** argv )
{
    if( !iv::IV::init( argc, argv ) )
        return 0;

    iv::util::createivD();

    iv::IV::exit();

    return 0;
}
