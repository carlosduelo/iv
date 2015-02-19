/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/factoryOctreeConstructor.h>

namespace iv
{

namespace DataHandler
{

OctreeConstructorPtr CreateOctreeConstructor( const unsigned type,
                                              const OctreeConstructorAttrPtr& oc,
                                              const level_t readL )
{
    switch( type )
    {
        case IV_OCTREE_CONSTRUCTOR_MEMORY:
            return OctreeConstructorPtr( new OctreeConstructorMemory( oc, readL ) );
        default:
            return OctreeConstructorPtr();
    }
}

}

}
