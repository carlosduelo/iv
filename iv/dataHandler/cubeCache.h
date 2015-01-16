/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CUBECACHE_H_
#define _IV_CUBECACHE_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileHandler.h>
#include <iv/dataHandler/lruLinkedList.h>
#include <iv/dataHandler/queue.hpp>

#include <thread>
#include <unordered_map>
#include <queue>

namespace iv
{

namespace DataHandler
{

class CubeCache
{
public:
    CubeCache() : _stopped( true ) {}

    bool init( const CacheAttrPtr& attr );

    void stop();

    const ObjectHandlerPtr get( const index_node_t id );

private:
    typedef std::unique_ptr< std::thread >                      threadPtr;
    typedef std::unordered_map< index_node_t, CacheObjectPtr >  table_t;

    bool _stopped;
    std::mutex  _mutex;

    CacheAttrPtr    _attr;
    FileHandlerPtr  _file;

    std::unique_ptr< float[] >  _data;
    std::unique_ptr< float[] >  _bufferPlane;

    LRULinkedListPtr    _lruList;

    table_t                     _cubesTable;

    threadPtr                   _toReadThread;
    Queue< CacheObjectPtr >     _toRead;

    bool _unlock( CacheObject* o );

    bool _lock( CacheObject* o );

    void _updateToRead();

    void _readProcess();
};

}

}

#endif // _IV_CUBECACHE_H_
