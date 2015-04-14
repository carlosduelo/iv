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
    Cache()
    {
    }

    bool init( const CacheAttrPtr& attr );

    void stop();

    const ObjectHandlerPtr get( const index_node_t id );

    // Data From Volume
    const vec3int32_t& getRealDimension() const
    {
        return _cache->getRealDimension();
    }

    level_t getnLevels() const
    {
        return _cache->getnLevels();
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
};

}

}

#endif // _IV_CACHE_H_
