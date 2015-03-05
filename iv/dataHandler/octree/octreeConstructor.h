/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_H_
#define _IV_OCTREE_CONSTRUCTOR_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/util/queue.hpp>
#include <iv/dataHandler/octree/octreeConstructorAttr.h>
#include <iv/dataHandler/octree/octreeConstructorStats.h>
#include <iv/dataHandler/octree/worker.h>
#include <iv/dataHandler/octree/dataWarehouse.h>

#include <thread>

namespace iv
{

namespace DataHandler
{

class OctreeConstructor
{
public:
    OctreeConstructor( const OctreeConstructorAttrPtr& octree,
                       const index_node_t cube )
               : _octree( octree )
               , _cube( cube )
    {
    }

    virtual ~OctreeConstructor() {}

    bool compute( const file_type_t& file_type,
                          const file_args_t& file_args );

    const OctreeConstructorStats& getStats() const
    {
        return _stats;
    }

protected:
    const OctreeConstructorAttrPtr  _octree;
    const index_node_t              _cube;

    DataWarehousePtr                _dataWarehouse;
    std::vector< WorkerPtr >        _workers;

    OctreeConstructorStats          _stats; 
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_H_
