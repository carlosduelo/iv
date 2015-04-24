/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_WORKER_H_
#define _IV_OCTREE_WORKER_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <iv/dataHandler/octree/octreeConstructorStats.h>
#include <thread>

namespace iv
{

namespace DataHandler
{

class Worker
{
public:
    Worker( DataWarehousePtr& data,
            CachePtr& cache,
            const level_t readLevel,
            OctreeConstructorStats& stats )
        : _cache( cache )
        , _data( data )
        , _readLevel( readLevel )
        , _stats( stats )
    {
    }

    virtual ~Worker() {}

    void addCube( const index_node_t cube );

    bool start();

    void wait();

protected:
    CachePtr                            _cache;
    DataWarehousePtr                    _data;
    const level_t                       _readLevel;
    OctreeConstructorStats&             _stats;
    std::vector< index_node_t >         _cubes;

    virtual bool _computeCube( const index_node_t cube ) = 0;

private:
    std::thread _thread;

    void _run();
};

}
}

#endif // _IV_OCTREE_WORKER_H_
