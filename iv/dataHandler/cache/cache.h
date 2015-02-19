/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CACHE_H_
#define _IV_CACHE_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/cache/controlCache.h>

namespace iv
{

namespace DataHandler
{

class Cache
{
public:
    Cache( const cache_type_t cacheType )
        : _cacheType ( cacheType )
    {
    }

    bool init( const CacheAttrPtr& attr );

    void stop();

    const ObjectHandlerPtr get( const index_node_t id );

    cache_type_t getTypeCache() const { return _cacheType; }

    // Data From Volume
    const vec3int32_t& getRealDimension() const
    {
        return _cache->getRealDimension();
    }

    const float * getGridX() const
    {
        return _cache->getGridX();
    }

    const float * getGridY() const
    {
        return _cache->getGridX();
    }

    const float * getGridZ() const
    {
        return _cache->getGridX();
    }
private:
    ControlCachePtr     _cache;
    const cache_type_t  _cacheType;
};

}

}

#endif // _IV_CACHE_H_
