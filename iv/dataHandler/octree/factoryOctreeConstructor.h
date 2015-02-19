/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_FACTORY_OCTREE_CONSTRUCTOR_H_
#define _IV_FACTORY_OCTREE_CONSTRUCTOR_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <iv/dataHandler/octree/octreeConstructorMemory.h>

namespace iv
{

namespace DataHandler
{

OctreeConstructorPtr CreateOctreeConstructor( const unsigned type,
                                              const OctreeConstructorAttrPtr& oc,
                                              const level_t readLevel );

}

}

#endif // _IV_FACTORY_OCTREE_CONSTRUCTOR_H_
