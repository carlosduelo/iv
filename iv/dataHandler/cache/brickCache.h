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
            const CacheAttrPtr&    attr,
            const CacheObjectPtr&  obj,
            const LRULinkedList::node_ref& data,
            std::function< void ( const CacheObjectPtr&, const bool ) > callback)
        : _cubeCache( cubeCache )
        , _attr( attr )
        , _obj( obj )
        , _data( data )
        , _callback( callback )
    {
    }

    void start();

private:
    std::thread _reader;
    const ControlCachePtr&          _cubeCache;
    const CacheAttrPtr&             _attr;
    const CacheObjectPtr            _obj;
    const LRULinkedList::node_ref   _data;
    std::function< void ( const CacheObjectPtr&, const bool ) > _callback;

    void _read();
};

typedef std::unique_ptr< Reader > ReaderPtr;

class BrickCache : public ControlCache
{
public:
    // Data From Volume
    virtual const vec3int32_t& getRealDimension() const;

    virtual level_t  getnLevels() const;

    virtual const float * getGridX() const { return _xGrid; }
                                                           
    virtual const float * getGridY() const { return _yGrid; }
                                                           
    virtual const float * getGridZ() const { return _zGrid; }
private:
    ControlCachePtr                                 _cubeCache;
    std::mutex                                      _mutex;
    std::condition_variable                         _cond;
    std::unordered_map< index_node_t, ReaderPtr >   _readers;
    
    float * _xGrid;
    float * _yGrid;
    float * _zGrid;

    bool _init();

    void _stop();

    void _readProcess( const CacheObjectPtr& obj,
                               const LRULinkedList::node_ref data );

    void _finishRead( const CacheObjectPtr& obj, const bool valid );
};

}

}

#endif // _IV_BRICKCACHE_H_
