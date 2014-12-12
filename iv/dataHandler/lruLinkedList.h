/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_LRU_LIST_H_
#define _IV_LRU_LIST_H_

#include <iv/dataHandler/cacheEntry.h>

namespace iv
{

namespace DataHandler
{

class LRULinkedList
{

public:
    LRULinkedList( const uint32_t size ) : _size( size ) {}

    ~LRULinkedList();

    bool init( const float* data, const uint32_t sizeElement );

    void stop( );

    const CacheEntryPtr getEmpty( );

    void remove( CacheEntryPtr& node );

    void elementAccesed( const CacheEntryPtr node );

private:
    class NodeList;
    typedef std::weak_ptr< NodeList > node_wptr;
    typedef std::shared_ptr< NodeList > node_ptr;

    const uint32_t _size;

    node_ptr        _list;
    node_wptr       _lastElement;

    void _elementAccessed( const node_wptr node );
};

}

}

#endif // _IV_LRU_LIST_H_
