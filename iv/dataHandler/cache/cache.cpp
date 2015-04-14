/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/cache.h>

#include <iv/common/global.h>

#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/cache/cubeCacheFactory.h>
#include <iv/dataHandler/cache/brickCache.h>

namespace iv
{

namespace DataHandler
{

bool Cache::init( const CacheAttrPtr& attr )
{
    const Global& global = Global::getGlobal();
    if( !global.useCuda() )
        _cache.reset( CreateCubeCache( attr->cubeCacheImpl ) );
#ifdef IV_USE_CUDA
    else
        _cache.reset( new BrickCache() );
#endif

    if( _cache )
        return _cache->init( attr );

    return false;
}

void Cache::stop()
{
    _cache->stop();
    _cache.reset();
}

const ObjectHandlerPtr Cache::get( const index_node_t id )
{
    if( _cache )
        return _cache->get( id );

    return ObjectHandlerPtr();
}

}

}
