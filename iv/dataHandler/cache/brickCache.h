/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_BRICKCACHE_H_
#define _IV_BRICKCACHE_H_

#include <iv/dataHandler/cache/controlCache.h>

#include <unordered_map>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace iv
{

namespace DataHandler
{

class Reader
{
public:
    Reader( const ControlCachePtr& cubeCache,
            const CacheObjectPtr&  obj,
            const LRULinkedList::node_ref& data,
            std::function< void ( const CacheObjectPtr& ) > callback)
        : _cubeCache( cubeCache )
        , _obj( obj )
        , _data( data )
        , _callback( callback )
    {
    }

    void start();
    
private:
    std::thread _reader;
    const ControlCachePtr&          _cubeCache;
    const CacheObjectPtr            _obj;
    const LRULinkedList::node_ref&  _data;
    std::function< void ( const CacheObjectPtr& ) > _callback;

    void _read();
};

typedef std::unique_ptr< Reader > ReaderPtr;

class BrickCache : public ControlCache
{
public:
    // Data From Volume
    virtual const vec3int32_t& getRealDimension() const;

    virtual const float * getGridX() const;

    virtual const float * getGridY() const;

    virtual const float * getGridZ() const;
private:
    ControlCachePtr            _cubeCache;
    std::mutex                                      _mutex;
    std::condition_variable                         _cond;
    std::unordered_map< index_node_t, ReaderPtr >   _readers;

    bool _init();

    void _stop();

    void _readProcess( const CacheObjectPtr& obj,
                               const LRULinkedList::node_ref data );

    void _finishRead( const CacheObjectPtr& obj );
};

}

}

#endif // _IV_BRICKCACHE_H_
