/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>

#include <iv/dataHandler/octree/octreeConstructor.h>
#include <iv/dataHandler/octree/octreeConstructorStats.h>
#include <iv/dataHandler/octree/workerCPU.h>

#include <iv/dataHandler/cache/cache.h>
#include <iv/dataHandler/cache/cacheAttr.h>

#include <iv/common/mortonCodeUtil_CPU.h>

#include <chrono>

namespace iv
{

namespace DataHandler
{

bool OctreeConstructor::compute()
{
    const Global& global = IV::getGlobal();

    level_t readLevel = 0;
#ifdef IV_USE_CUDA
    if( global.useCuda() )
        readLevel = global.getBrickLevel();
    else
#endif
    readLevel = global.getCubeLevel();

    index_node_t idStart =
            _cube <<
            3 * ( readLevel - _constructorLevel );
    index_node_t idFinish =
            ( ( _cube + 1 ) <<
            3 * ( readLevel - _constructorLevel ) ) - 1;

    index_node_t min =
            _cube <<
            3 * ( global.getOctreeLevel() - _constructorLevel );
    index_node_t max =
            ( ( _cube + 1 ) <<
            3 * ( global.getOctreeLevel() - _constructorLevel ) ) - 1;

    // Create DataWarehouse
    _dataWarehouse = DataWarehousePtr( new DataWarehouse( min, max ) );

    // Create Cache
    CacheAttrPtr cacheAttr( new iv::DataHandler::CacheAttr() );

    CachePtr cache( new iv::DataHandler::Cache( ) );

    if( ! cache )
        return false;

    class Data
    {
    public:
        Data( DataWarehousePtr& dPtr,
              CachePtr& cPtr,
              CacheAttrPtr& cAttr,
              OctreeConstructorStats& stats )
            : _dPtr( dPtr )
            , _cPtr( cPtr )
            , _stats( stats )
        {
            wasFine = _dPtr->start() && _cPtr->init( cAttr );
        }
        ~Data()
        {
            auto startC = std::chrono::high_resolution_clock::now();
            _dPtr->wait();
            auto endC = std::chrono::high_resolution_clock::now();
            _stats.incSortingTime( std::chrono::duration_cast<
                                                    std::chrono::milliseconds>(
                                                        endC - startC ) );
            _cPtr->stop();
        }

        bool wasFine;
    private:
        DataWarehousePtr&           _dPtr;
        CachePtr&                   _cPtr;
        OctreeConstructorStats&     _stats;
    };
    Data data( _dataWarehouse, cache, cacheAttr, _stats );

    if( !data.wasFine )
        return false;

    bool wasFine = true;
    if( global.useHyperThreading() )
    {
        uint32_t worker = 0;
        for( index_node_t id = idStart; id <= idFinish; id++ )
        {
            // Check cube is outside of volume
            vec3int32_t coordCubeStart = getMinBoxIndex2( id,
                                                          readLevel,
                                                          global.getnLevels() );

            if( coordCubeStart.x() < cache->getRealDimension().x()  &&
                coordCubeStart.y() < cache->getRealDimension().y()  &&
                coordCubeStart.z() < cache->getRealDimension().z() )
            {
#ifdef IV_USE_CUDA
                if( global.useCuda() )
                    _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                                  cache,
                                                                  readLevel,
                                                                  _stats ) ) );
                else
#endif
                    _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                                  cache,
                                                                  readLevel,
                                                                  _stats ) ) );
                _workers[ worker ]->addCube( id );
                worker++;
            }
        }
    }
    else
    {
        uint32_t numCubes = 0;
        for( index_node_t id = idStart; id <= idFinish; id++ )
        {
            // Check cube is outside of volume
            vec3int32_t coordCubeStart = getMinBoxIndex2( id,
                    readLevel,
                    global.getnLevels() );

            if( coordCubeStart.x() < cache->getRealDimension().x()  &&
                coordCubeStart.y() < cache->getRealDimension().y()  &&
                coordCubeStart.z() < cache->getRealDimension().z() )
            {
                numCubes++;
            }
        }

        uint32_t eachThread = numCubes / global.getMaxNumThreads();
        index_node_t id = idStart;
        uint32_t worker = 0;
        while( id <= idFinish )
        {
#ifdef IV_USE_CUDA
            if( global.useCuda() )
                _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                              cache,
                                                              readLevel,
                                                              _stats ) ) );
#endif
            else
                _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                              cache,
                                                              readLevel,
                                                              _stats ) ) );
            uint32_t cubes = 0;
            while( cubes <= eachThread && id <= idFinish )
            {
                vec3int32_t coordCubeStart = getMinBoxIndex2( id,
                        readLevel,
                        global.getnLevels() );

                if( coordCubeStart.x() < cache->getRealDimension().x()  &&
                    coordCubeStart.y() < cache->getRealDimension().y()  &&
                    coordCubeStart.z() < cache->getRealDimension().z() )
                {
                    cubes++;
                    _workers[ worker ]->addCube( id );
                }
                id++;
            }
            worker++;
        }
    }

    for( uint32_t w = 0; w < _workers.size() && wasFine; w++ )
        wasFine &= _workers[w]->start();

    if( !wasFine )
        return false;

    for( uint32_t w = 0; w < _workers.size() && wasFine; w++ )
        _workers[w]->wait();

    return true;
}

}

}
