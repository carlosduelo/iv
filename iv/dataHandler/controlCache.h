/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CONTROLCACHE_H_
#define _IV_CONTROLCACHE_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/lruLinkedList.h>
#include <iv/dataHandler/queue.hpp>

#include <thread>
#include <unordered_map>
#include <queue>

namespace iv
{

namespace DataHandler
{

class ControlCache
{
public:
    ControlCache() : _stopped( true ) {}
    virtual ~ControlCache() {};

    bool init( const CacheAttrPtr& attr );

    void stop();

    const ObjectHandlerPtr get( const index_node_t id );

private:
    typedef std::unique_ptr< std::thread >  threadPtr;

    bool _stopped;
    std::mutex  _mutex;

    LRULinkedListPtr    _lruList;

    threadPtr                   _toReadThread;
    Queue< CacheObjectPtr >     _toRead;

    bool _unlock( CacheObject* o );

    bool _lock( CacheObject* o );

    void _updateToRead();

protected:
    CacheAttrPtr    _attr;

    unsigned _numElements;
    float *  _data;

    typedef std::unordered_map< index_node_t, CacheObjectPtr >  table_t;
    table_t                     _cubesTable;

    virtual bool _init() = 0;

    virtual void _stop() = 0;

    virtual void _readProcess( const CacheObjectPtr& obj,
                               const LRULinkedList::node_ref data ) = 0;
};

}

}

#endif // _IV_CONTROLCACHE_H_
