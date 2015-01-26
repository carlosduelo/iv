/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_LRU_LIST_H_
#define _IV_LRU_LIST_H_

#include <iv/common/types.h>

#include <memory>
#include <condition_variable>

namespace iv
{

namespace DataHandler
{

class LRULinkedList
{
private:
    class NodeList;
    typedef std::weak_ptr< NodeList > node_wptr;
    typedef std::shared_ptr< NodeList > node_ptr;

    class NodeRef
    {
    public:
        NodeRef( const index_node_t id );
        NodeRef( );

        const float* get() const;

        index_node_t getID() const;

        bool isValid() const;

    private:
        friend class LRULinkedList;

        NodeRef( const node_wptr n, const index_node_t id );

        node_wptr       _ptr;
        index_node_t    _id;
    };

public:
    typedef NodeRef node_ref;

    LRULinkedList() : _stopped( true ) {}

    bool init( const uint32_t size, const float* data,
                                        const uint32_t sizeElement );

    void stop( );

    bool getEmptySync( node_ref& node );

    bool getEmpty( node_ref& node );

    void remove( node_ref& ref );

    void elementAccesed( const node_ref& ref );

private:
    uint32_t        _size;

    node_ptr        _list;
    node_wptr       _lastElement;

    std::mutex              _mutex;
    std::condition_variable _cond;

    bool                    _stopped;

    void _elementAccessed( const node_wptr node );

    bool _getEmpty( node_ref& _ref );
};

}

}
#endif // _IV_LRU_LIST_H_
