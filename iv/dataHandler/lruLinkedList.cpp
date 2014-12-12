/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/lruLinkedList.h>

namespace iv
{

namespace DataHandler
{

class LRULinkedList::NodeList : public CacheEntry
{
public:
    NodeList( const float*  e)
        : CacheEntry( e )
        , _free( true )
    {
    }
    bool            _free;
    iv::DataHandler::LRULinkedList::node_ptr        _next;
    iv::DataHandler::LRULinkedList::node_wptr       _previus;
    iv::DataHandler::LRULinkedList::node_wptr       _oneself;
};

LRULinkedList::~LRULinkedList()
{
}

bool LRULinkedList::init( const float* data, const uint32_t sizeElement )
{
    _list.reset( new NodeList( data ) );
    node_ptr node = _list;
    node->_oneself = _list;
    for( unsigned i = 1; i < _size; i++ )
    {
        node->_next.reset( new NodeList( data + i * sizeElement ) );
        node->_oneself = node;
        node_ptr previus = node;
        node = node->_next;
        node->_previus = previus;
        node->_oneself = node;
    }
    _lastElement = node;

    return true;
}

void LRULinkedList::stop( )
{
    // Destroy starting for lastElment
    node_ptr n = _lastElement.lock();
    _lastElement.reset();
    while( !n->_previus.expired() )
    {
        n = n->_previus.lock();
        n->_next.reset();
    }
    _list.reset();
}

const CacheEntryPtr LRULinkedList::getEmpty( )
{
    if( !_list->_free )
        return CacheEntryPtr();

    node_ptr node = _list;
    node->_free = false;
    _elementAccessed( node );

    return node;
}

void LRULinkedList::remove( CacheEntryPtr& node )
{
    assert( node );

    NodeList * ptr = static_cast< NodeList* >( node.get() );
    node = CacheEntryPtr();
    node_ptr n = ptr->_oneself.lock();
    assert( n );

    if( !n->_next ) // Last Element
    {
        _lastElement = n->_previus;
        n->_previus.reset();
        node_ptr l = _lastElement.lock();
        l->_next.reset();

        n->_next = _list;
        _list->_previus = n;
        _list = n;
    }
    else if ( !n->_previus.expired() )
    {
        node_ptr l = n->_previus.lock();
        node_ptr r = n->_next;
        n->_next.reset();
        n->_previus.reset();
        l->_next = r;
        r->_previus = l;

        n->_next = _list;
        _list->_previus = n;
        _list = n;
    }
    // Firs Element do nothing

    n->_free = true;
}

void LRULinkedList::elementAccesed( const CacheEntryPtr node )
{
    assert( node );

    NodeList * ptr = static_cast< NodeList* >( node.get() );
    node_ptr n = ptr->_oneself.lock();
    assert( n );
    _elementAccessed( n );
}

void LRULinkedList::_elementAccessed( const node_wptr node )
{
    node_ptr n = node.lock();
    if( n->_previus.expired() ) // First element
    {
        _list = n->_next;
        n->_next.reset();
        _list->_previus.reset();

        // Insert last
        node_ptr l = _lastElement.lock();
        l->_next = n;
        n->_previus = l;
        _lastElement = n;
    }
    else if ( n->_next ) // Middle element
    {
        node_ptr l = n->_previus.lock();
        node_ptr r = n->_next;
        l->_next = r;
        r->_previus = l;
        n->_next.reset();
        n->_previus.reset();

        // Insert last
        l = _lastElement.lock();
        l->_next = n;
        n->_previus = l;
        _lastElement = n;
    }
        // Last Element do nothing

}

}

}
