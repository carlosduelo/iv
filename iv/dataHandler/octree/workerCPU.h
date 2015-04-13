/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_WORKER_CPU_H_
#define _IV_OCTREE_WORKER_CPU_H_

#include <iv/dataHandler/octree/worker.h>

namespace iv
{

namespace DataHandler
{

class WorkerCPU : public Worker
{
public:
    WorkerCPU( DataWarehousePtr& data,
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
                        vec3int32_t    coordStartData,
                        const uint32_t dimCubeData );
};

}
}

#endif // _IV_OCTREE_WORKER_CPU_H_
