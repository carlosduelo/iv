/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/brickCache.h>

namespace iv
{

namespace DataHandler
{

bool BrickCache::_init()
{
    return true;
}

void BrickCache::_stop()
{
}

void BrickCache::_readProcess( const CacheObjectPtr& /*obj*/,
                           const LRULinkedList::node_ref /*data*/ )
{
}

}

}
