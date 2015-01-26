/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_BRICKCACHE_H_
#define _IV_BRICKCACHE_H_

#include <iv/dataHandler/cache/controlCache.h>

namespace iv
{

namespace DataHandler
{

class BrickCache : public ControlCache
{
private:
    ControlCachePtr _cubeCache;

    bool _init();

    void _stop();

    void _readProcess( const CacheObjectPtr& obj,
                               const LRULinkedList::node_ref data );
};

}

}

#endif // _IV_BRICKCACHE_H_

