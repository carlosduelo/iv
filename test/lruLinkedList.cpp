/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/lruLinkedList.h>

#include <iostream>
#include <unordered_map>
#include <thread>

#define NUM_ELEM 100
#define SIZE_ELEM 10

iv::DataHandler::LRULinkedListPtr           lruList;
iv::DataHandler::LRULinkedList::node_ref    ref;

void thread_work()
{
    ref = iv::DataHandler::LRULinkedList::node_ref( 101 );
    lruList->getEmpty( ref );
}

int main( int , char ** )
{
    lruList.reset( new iv::DataHandler::LRULinkedList() );

    std::unique_ptr< float[] > data( new float[ NUM_ELEM * SIZE_ELEM ] );
    std::unordered_map< iv::index_node_t,
                            iv::DataHandler::LRULinkedList::node_ref > elements;

    lruList->init( NUM_ELEM, data.get(), SIZE_ELEM );

    // TEST 1
    {
        float* ptr = data.get();
        for( iv::index_node_t id = 0; id < NUM_ELEM; id++ )
        {
            iv::DataHandler::LRULinkedList::node_ref    refD( id );
            lruList->getEmpty( refD );
            assert( refD.get() );
            assert( refD.getID() == id );
            elements[id] = refD;
            assert( elements[id].get() == ptr );
            assert( elements[id].getID() == id );
            ptr += SIZE_ELEM;
        }

        lruList->getEmpty( ref );
        assert( ref.get() == 0 && ref.getID() == 0 );
    }

    // TEST 2
    {
        const float* ptr = elements[50].get();
        lruList->remove( elements[50] );
        assert( elements[50].get() == 0 );
        iv::DataHandler::LRULinkedList::node_ref    refD( 100 );
        lruList->getEmpty( refD );
        elements[100] = refD;
        assert( ptr == elements[100].get() );
    }

    // TEST 3
    {
        std::thread _t( thread_work );

        lruList->remove( elements[5] );
        _t.join();

        assert( ref.getID() == 101 );

        lruList->remove( ref );

        assert( ref.get() == 0 && ref.getID() == 0 );
    }

    lruList->stop();
}
