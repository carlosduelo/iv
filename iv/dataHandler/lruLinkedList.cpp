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

class LRULinkedList::NodeList
{
public:
    NodeList( const float*  e)
        : _element( e )
        , _free( true )
    {
    }

    const float *   _element;
    bool            _free;
    iv::DataHandler::LRULinkedList::node_ptr        _next;
    iv::DataHandler::LRULinkedList::node_wptr       _previus;
};


LRULinkedList::NodeRef::NodeRef( const node_wptr n, const index_node_t id )
    : _ptr( n )
    , _id( id )
{
}

LRULinkedList::NodeRef::NodeRef( const index_node_t id )
    : _id( id )
{
}

LRULinkedList::NodeRef::NodeRef( )
    : _id( 0 )
{
}

const float* LRULinkedList::NodeRef::get() const
{
    node_ptr n = _ptr.lock();
    if( !n )
        return 0;
    return n->_element;
}

index_node_t LRULinkedList::NodeRef::getID() const
{
    return _id;
}

bool LRULinkedList::NodeRef::isValid() const
{
    return !_ptr.expired();
}


bool LRULinkedList::init( const uint32_t size, const float* data,
                                                const uint32_t sizeElement )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    _size = size;
    _list.reset( new NodeList( data ) );
    node_ptr node = _list;
    for( unsigned i = 1; i < _size; i++ )
    {
        node->_next.reset( new NodeList( data + i * sizeElement ) );
        node_ptr previus = node;
        node = node->_next;
        node->_previus = previus;
    }
    _lastElement = node;

    _stopped = false;

    return true;
}

void LRULinkedList::stop( )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    _stopped = true;

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

void LRULinkedList::getEmptySync( node_ref& node )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    while( !_stopped && !_list->_free )
        _cond.wait( mlock );

    if( _stopped )
        return;

    _getEmpty( node );
}

void LRULinkedList::getEmpty( node_ref& node )
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( !_list->_free || _stopped )
        return;

    _getEmpty( node );
}

void LRULinkedList::_getEmpty( node_ref& ref )
{
    node_ptr node = _list;
    node->_free = false;
    _elementAccessed( node );

    ref._ptr = node;
}

void LRULinkedList::remove( node_ref& ref )
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( _stopped )
        return;

    node_ptr n = ref._ptr.lock();
    ref._ptr.reset();
    ref._id = 0;
    if( !n )
        return;

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
    mlock.unlock();
    _cond.notify_one();
}

void LRULinkedList::elementAccesed( const node_ref& ref )
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( _stopped )
        return;

    node_ptr n = ref._ptr.lock();
    if( !n )
        return;
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
