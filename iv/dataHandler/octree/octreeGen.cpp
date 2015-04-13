/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeGen.h>
#include <iv/dataHandler/octree/octreeConstructor.h>

#include <iv/common/mortonCodeUtil_CPU.h>

#include <algorithm>

namespace iv
{

namespace DataHandler
{

bool OctreeGen::compute( std::vector< index_node_t >& cubes )
{
    // Sort cubes
    std::sort( cubes.begin(), cubes.end() );

    OctreeConstructorAttrPtr attr( new OctreeConstructorAttr( _attr ) );

    std::vector< OctreeConstructorPtr > constructors;
    constructors.resize( cubes.size() );
    for( uint32_t i = 0; i < cubes.size(); i++ )
    {
        constructors[i].reset( new OctreeConstructor( attr, cubes[i] ) );
    }

    for( auto c = constructors.begin(); c != constructors.end(); ++c )
        (*c)->compute();

    return true;
}

}

}
