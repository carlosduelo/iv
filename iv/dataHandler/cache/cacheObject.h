/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CACHE_OBJECT_H_
#define _IV_CACHE_OBJECT_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/util/lruLinkedList.h>

#include <memory>
#include <condition_variable>
#include <functional>

namespace iv
{

namespace DataHandler
{

class CacheObject;

class CacheObject : public std::enable_shared_from_this< CacheObject >
{
public:
    const float* try_lock();

    const float* lock();

    void unlock();

    index_node_t getID() const;

private:
    friend class ControlCache;
    friend class CubeCacheSimple;
    friend class BrickCache;

    enum State { CACHED, NO_CACHED, READING };
    typedef std::function< bool ( CacheObject* ) > CacheObjectCallback_t;

    CacheObject( const index_node_t id,
                 const CacheObjectCallback_t  lockCallback,
                 const CacheObjectCallback_t  unlockCallback );

    void setData( const LRULinkedList::node_ref& data);

    State getState();

    void setState( const State& state );

    void waitUnlocked();

    ObjectHandlerPtr createHandler();

    State                   _state;
    uint64_t                _nRefs;
    const index_node_t      _id;
    LRULinkedList::node_ref _data;

    const CacheObjectCallback_t  _lockCallback;
    const CacheObjectCallback_t  _unlockCallback;

    std::mutex              _mutex;
    std::condition_variable _condState;
    std::condition_variable _condRefs;
};

}

}
#endif // _IV_CACHE_OBJECT_H_
