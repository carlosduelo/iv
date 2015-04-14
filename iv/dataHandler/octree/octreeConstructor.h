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
               , _nLevels( 0 )
    {
    }

    virtual ~OctreeConstructor() {}

    bool compute();

    const OctreeConstructorStats& getStats() const
    {
        return _stats;
    }

    const DataWarehousePtr getData() const
    {
        return _dataWarehouse;
    }

    level_t getnLevels() const
    {
        return _nLevels;
    }

protected:
    const OctreeConstructorAttrPtr  _octree;
    const index_node_t              _cube;
    level_t                         _nLevels;

    DataWarehousePtr                _dataWarehouse;
    std::vector< WorkerPtr >        _workers;

    OctreeConstructorStats          _stats;
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_H_
