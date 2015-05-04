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

namespace iv
{

namespace DataHandler
{

void OctreeGen::printStats() const
{
    std::cout << "Octree Construction Stats:" << std::endl;
    std::cout << "\tData read: " << _stats.getBytesRead() / 1024 / 1024 << " MB" << std::endl;
    std::cout << "\tCubes computed: " << _stats.getCubesComputed() << std::endl;
    std::cout << "\tTime Reading: " << _stats.getReadingTime().count() / 1000 << " sec" << std::endl;
    std::cout << "\tBandwidth: "
              << ( _stats.getBytesRead() / 1024 / 1024 ) / _stats.getReadingTime().count() / 1000
              << " MB/s" << std::endl;
    std::cout << "\tTime Computing: " << _stats.getComputingTime().count() / 1000 << " sec" << std::endl;
    std::cout << "\tTime Sorting: " << _stats.getSortingTime().count() / 1000 << " sec" << std::endl;
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

    // Sort cubes
    std::sort( cubes.begin(), cubes.end() );

    std::vector< OctreeConstructorPtr > constructors;
    constructors.resize( cubes.size() );
    for( uint32_t i = 0; i < cubes.size(); i++ )
        constructors[i].reset( new OctreeConstructor( _constructorLevel,
                                                      cubes[i] ) );

    for( auto c = constructors.begin(); c != constructors.end(); ++c )
        (*c)->compute();

    // Collect octreeStatistics
    for( auto c = constructors.begin(); c != constructors.end(); ++c )
    {
        _stats.incReadBytes( (*c)->getStats().getBytesRead() );
        _stats.incCube( (*c)->getStats().getCubesComputed() );
        _stats.incReadingTime( (*c)->getStats().getReadingTime() );
        _stats.incComputingTime( (*c)->getStats().getComputingTime() );
        _stats.incSortingTime( (*c)->getStats().getSortingTime() );
    }

    uint32_t numRanges = 0;
    uint32_t maxHeight = 0;
    for( auto c = constructors.begin(); c != constructors.end(); ++c )
    {
        numRanges += (*c)->getData()->getNumRanges();
        maxHeight = (*c)->getData()->getMaxHeight() > maxHeight
                        ? (*c)->getData()->getMaxHeight()
                        : maxHeight;
    }

    // Write to file
    std::ofstream file( global.getOctreeFile().c_str(), std::ofstream::binary );

    level_t nLevels = global.getnLevels();
    level_t level = global.getOctreeLevel();
    uint32_t cubeInc = global.getCubeInc();
    file.write( (char*)&nLevels, sizeof( nLevels ) );
    file.write( (char*)&level, sizeof( level ) );
    file.write( (char*)&maxHeight, sizeof( maxHeight ) );
    file.write( (char*)&cubeInc, sizeof( cubeInc ) );
    file.write( (char*)&numRanges, sizeof( numRanges ) );

    for( auto c = constructors.begin(); c != constructors.end(); ++c )
    {
        const uint32_t numR = (*c)->getData()->getNumRanges();
        const std::string file_name = (*c)->getData()->getFileData();
        std::vector< index_node_t > data;
        data.reserve( 2 * numR );
        std::ifstream fileB;
        fileB.open( file_name.c_str(), std::ifstream::binary );
        fileB.read( (char*)data.data(), 2 * numR *sizeof( index_node_t ) );
        fileB.close();
        file.write( (char*)data.data(), 2 * numR *sizeof( index_node_t ) );
    }

    file.close();

    return true;
}

}

}
