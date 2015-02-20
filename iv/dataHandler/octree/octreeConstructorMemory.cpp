/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructorMemory.h>

#include <iv/dataHandler/cache/cache.h>
#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/cache/objectHandler.h>
#include <iv/common/mortonCodeUtil_CPU.h>

namespace
{

void computeCube( const iv::index_node_t ,//id,
                  const float * ,//cube,
                  const iv::level_t ,//readLevel,
                  const iv::DataHandler::OctreeConstructorAttrPtr& )//octree )
{
    

}
}

namespace iv
{

namespace DataHandler
{

bool OctreeConstructorMemory::start( const file_type_t& file_type,
                                     const file_args_t& file_args )
{
    // Setting up cache
    CacheAttrPtr cacheAttr( new iv::DataHandler::CacheAttr() );

    cacheAttr->file_type = file_type;
    cacheAttr->file_args = file_args;

    cacheAttr->sizeCacheCPU = 100 * 1024 * 1024; // 100MB

    cacheAttr->offset = _octree->getOffset();
    cacheAttr->nLevels = _octree->getnLevels();
    cacheAttr->cubeLevel = _octree->getLevel();
    cacheAttr->cubeInc = 0;

    CachePtr cache( new iv::DataHandler::Cache( IV_CUBE_CACHE ) );

    if( !cache->init( cacheAttr ) )
        return false;

    _data.reserve( _octree->getLevel() + 1 );


    uint32_t dimension = pow( 2, _octree->getnLevels() );

    index_node_t idStart = coordinateToIndex( vec3int32_t(0,0,0),
                                              _readLevel,
                                              _octree->getnLevels() );

    index_node_t idFinish = coordinateToIndex( vec3int32_t( dimension-1,
                                                            dimension-1,
                                                            dimension-1),
                                               _readLevel,
                                               _octree->getnLevels() );

    std::cout << idStart << " " << idFinish << std::endl;
    for( index_node_t id = idStart; id <= idFinish; id++)
    {
        // Check cube is outside of volume
        vec3int32_t coordCubeStart = getMinBoxIndex2( id,
                                                     _readLevel,
                                                     _octree->getnLevels() );

        if( coordCubeStart.x() + _octree->getOffset().x() <
                                               cache->getRealDimension().x()  && 
            coordCubeStart.y() + _octree->getOffset().y() <
                                               cache->getRealDimension().y()  && 
            coordCubeStart.z() + _octree->getOffset().z() <
                                               cache->getRealDimension().z() )
        {
            ObjectHandlerPtr o = cache->get( id );
            const float* data = o->lock();
            std::cout << id << std::endl;
            computeCube( id, data, _readLevel, _octree );
            break;
        }
    }

    cache->stop();

    return true;
}

const OctreePtr OctreeConstructorMemory::stop()
{
    return OctreePtr();
}

}

}
