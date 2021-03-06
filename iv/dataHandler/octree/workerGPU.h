/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_WORKER_GPU_H_
#define _IV_OCTREE_WORKER_GPU_H_

#include <iv/dataHandler/octree/worker.h>

namespace iv
{

namespace DataHandler
{

class WorkerGPU : public Worker
{
public:
    WorkerGPU( DataWarehousePtr& data,
               CachePtr& cache,
               const OctreeConstructorAttrPtr& attr,
               OctreeConstructorStats& stats )
        : Worker( data, cache, attr, stats )
    {
    }

private:
    virtual bool _computeCube( const index_node_t cube );

    void _computeCubeData( const index_node_t id, const float* cube );

    bool _computeCube( const index_node_t id, const float* cube,
                        const uint32_t dimCube,
                        const uint32_t dimCubeData );
};

}
}

#endif // _IV_OCTREE_WORKER_GPU_H_
