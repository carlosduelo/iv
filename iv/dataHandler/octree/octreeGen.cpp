/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeGen.h>

#include <iv/common/global.h>
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
        return true;
        
    // Sort cubes
    std::sort( cubes.begin(), cubes.end() );

    OctreeConstructorAttrPtr attr( new OctreeConstructorAttr( _attr ) );

    std::vector< OctreeConstructorPtr > constructors;
    constructors.resize( cubes.size() );
    for( uint32_t i = 0; i < cubes.size(); i++ )
        constructors[i].reset( new OctreeConstructor( attr, cubes[i] ) );

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
    std::ofstream file( _attr->getFilePath().c_str(), std::ofstream::binary );

    const Global& global = Global::getGlobal();
    level_t nLevels = (*constructors.begin())->getnLevels();
    level_t level = _attr->getLevel(); 
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
