/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructor.h>
#include <iv/dataHandler/octree/octreeConstructorStats.h>
#include <iv/dataHandler/octree/workerCPU.h>

#include <iv/dataHandler/cache/cache.h>
#include <iv/dataHandler/cache/cacheAttr.h>

#include <iv/common/mortonCodeUtil_CPU.h>

namespace iv
{

namespace DataHandler
{

bool OctreeConstructor::compute( const file_type_t& file_type,
                                  const file_args_t& file_args )
{
    if( _octree->getConstructorLevel() > _octree->getLevel() )
    {
        std::cerr << "Contructor level may be <= octree level" << std::endl;
        return false;
    }
    if( _octree->getConstructorLevel() > _octree->getReadLevel() )
    {
        std::cerr << "Contructor level may be <= read level" << std::endl;
        return false;
    }

    if( _octree->getReadLevel() > _octree->getLevel() )
    {
        std::cerr << "Read level may be <= octree level" << std::endl;
        return false;
    }

    index_node_t idStart =
            _cube <<
            3 * ( _octree->getReadLevel() - _octree->getConstructorLevel() );
    index_node_t idFinish =
            ( ( _cube + 1 ) <<
            3 * ( _octree->getReadLevel() - _octree->getConstructorLevel() ) ) - 1;

    index_node_t min =
            _cube <<
            3 * ( _octree->getLevel() - _octree->getConstructorLevel() );
    index_node_t max =
            ( ( _cube + 1 ) <<
            3 * ( _octree->getLevel() - _octree->getConstructorLevel() ) ) - 1;

    // Create DataWarehouse
    _dataWarehouse = DataWarehousePtr( new DataWarehouse( min, max ) );

    // Create Cache
    CacheAttrPtr cacheAttr( new iv::DataHandler::CacheAttr() );

    cacheAttr->file_type = file_type;
    cacheAttr->file_args = file_args;

    cacheAttr->sizeCacheCPU = 1000 * 1024 * 1024; // 100MB
    cacheAttr->sizeCacheGPU = 1000 * 1024 * 1024; // 100MB

    cacheAttr->offset = _octree->getOffset();
    cacheAttr->nLevels = _octree->getnLevels();
    cacheAttr->cubeLevel = _octree->getReadLevel();
    cacheAttr->cubeInc = _octree->getCubeInc();

    CachePtr cache;
    if( _octree->useCuda() )
        cache.reset( new iv::DataHandler::Cache( IV_BRICK_CACHE ) );
    else
        cache.reset( new iv::DataHandler::Cache( IV_CUBE_CACHE ) );

    if( ! cache )
        return false;

    class Data
    {
    public:
        Data( DataWarehousePtr& dPtr, CachePtr& cPtr, CacheAttrPtr& cAttr )
            : _dPtr( dPtr )
            , _cPtr( cPtr )
        {
            wasFine = _dPtr->start() && _cPtr->init( cAttr );
        }
        ~Data()
        {
            _dPtr->wait();
            _cPtr->stop();
        }

        bool wasFine;
    private:
        DataWarehousePtr&   _dPtr;
        CachePtr&           _cPtr;
    };
    Data data( _dataWarehouse, cache, cacheAttr );

    if( !data.wasFine )
        return false;

    bool wasFine = true;
    if( _octree->useHyperThreading () )
    {
        uint32_t worker = 0;
        for( index_node_t id = idStart; id <= idFinish; id++ )
        {
            // Check cube is outside of volume
            vec3int32_t coordCubeStart = getMinBoxIndex2( id,
                    _octree->getReadLevel(),
                    _octree->getnLevels() );

            if( coordCubeStart.x() + _octree->getOffset().x() <
                    cache->getRealDimension().x()  &&
                    coordCubeStart.y() + _octree->getOffset().y() <
                    cache->getRealDimension().y()  &&
                    coordCubeStart.z() + _octree->getOffset().z() <
                    cache->getRealDimension().z() )
            {
                if( _octree->useCuda() )
                    _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                                  cache,
                                                                  _octree,
                                                                  _stats ) ) );
                else
                    _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                                  cache,
                                                                  _octree,
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
                    _octree->getReadLevel(),
                    _octree->getnLevels() );

            if( coordCubeStart.x() + _octree->getOffset().x() <
                    cache->getRealDimension().x()  &&
                    coordCubeStart.y() + _octree->getOffset().y() <
                    cache->getRealDimension().y()  &&
                    coordCubeStart.z() + _octree->getOffset().z() <
                    cache->getRealDimension().z() )
            {
                numCubes++;
            }
        }

        uint32_t eachThread = numCubes / _octree->getNumThreads();
        index_node_t id = idStart;
        uint32_t worker = 0;
        while( id <= idFinish )
        {
            if( _octree->useCuda() )
                _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                              cache,
                                                              _octree,
                                                              _stats ) ) );
            else
                _workers.push_back( WorkerPtr( new WorkerCPU( _dataWarehouse,
                                                              cache,
                                                              _octree,
                                                              _stats ) ) );
            uint32_t cubes = 0;
            while( cubes <= eachThread && id <= idFinish )
            {
                vec3int32_t coordCubeStart = getMinBoxIndex2( id,
                        _octree->getReadLevel(),
                        _octree->getnLevels() );

                if( coordCubeStart.x() + _octree->getOffset().x() <
                        cache->getRealDimension().x()  &&
                        coordCubeStart.y() + _octree->getOffset().y() <
                        cache->getRealDimension().y()  &&
                        coordCubeStart.z() + _octree->getOffset().z() <
                        cache->getRealDimension().z() )
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
