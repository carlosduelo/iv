/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_H_
#define _IV_OCTREE_CONSTRUCTOR_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/octree/octree.h>
#include <iv/dataHandler/octree/octreeConstructorAttr.h>

namespace iv
{

namespace DataHandler
{

class OctreeConstructor
{
public:
    OctreeConstructor( const OctreeConstructorAttrPtr& octree,
                       const level_t readLevel )
               : _octree( octree )
               , _readLevel( readLevel )
    {
    }

    virtual ~OctreeConstructor() {}

    virtual bool start( const file_type_t& file_type,
                        const file_args_t& file_args ) = 0;

    virtual const OctreePtr stop() = 0;

protected:
    const OctreeConstructorAttrPtr _octree;
    const level_t   _readLevel;
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_H_
