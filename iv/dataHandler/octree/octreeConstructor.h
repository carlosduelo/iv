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
    OctreeConstructor( const level_t constructorLevel,
                       const index_node_t cube )
               : _constructorLevel( constructorLevel )
               , _cube( cube )
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

protected:
    const level_t                   _constructorLevel;
    const index_node_t              _cube;

    DataWarehousePtr                _dataWarehouse;
    std::vector< WorkerPtr >        _workers;

    OctreeConstructorStats          _stats;
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_H_
