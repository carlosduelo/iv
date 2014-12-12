/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/lruLinkedList.h>
#include <iostream>
#include <unordered_map>

#define NUM_ELEM 100
#define SIZE_ELEM 10

int main( int , char ** )
{
    iv::DataHandler::LRULinkedList lruList( NUM_ELEM );

    std::unique_ptr< float[] > data( new float[ NUM_ELEM * SIZE_ELEM ] );
    std::unordered_map< iv::index_node_t,
                        iv::DataHandler::CacheEntryPtr > elements;

    lruList.init( data.get(), SIZE_ELEM );

    // TEST 1
    {
        float* ptr = data.get();
        for( iv::index_node_t id = 0; id < NUM_ELEM; id++ )
        {
            iv::DataHandler::CacheEntryPtr c = lruList.getEmpty();
            assert( c->isValid() );
            assert( c.use_count() == 2 );
            elements[id] = c;
            assert( c.use_count() == 3 );
            c.reset();
            assert( elements[id].use_count() == 2 );
            assert( elements[id]->get() == ptr );
            ptr += SIZE_ELEM;
        }

        assert( !lruList.getEmpty() );
    }

    // TEST 2 
    {
        const float* ptr = elements[50]->get();
        lruList.remove( elements[50] );
        assert( !elements[50] );
        elements[100] = lruList.getEmpty();
        assert( elements[100].use_count() == 2 );
        assert( ptr == elements[100]->get() );
    }


    lruList.stop();
    #if 0

    std::shared_ptr< node_t > nodesP( );
    node_t * nodes = new node_t[ NUM_ELEM ];

    // Get Element
    nodes[0] = lruList.getEmpty( 0 ); 
    assert( lruList.getValue( nodes[0] ) == data.get() );

    // Accesed
    lruList.elementAccessed( nodes[0] );

    for(size_t i=1; i < NUM_ELEM; i++)
    {
        nodes[i] = lruList.getEmpty( i );
        assert( lruList.getValue( nodes[i] ) == data.get() + i * SIZE_ELEM );
        if( i % 2 )
            lruList.elementAccessed( nodes[i] );
    }

    const size_t m = NUM_ELEM/ 2;

    for(size_t i=0; i < m; i++)
    {
        lruList.remove( nodes[i] );
    }

    for(size_t i=1; i < m; i++)
    {
        nodes[i] = lruList.getEmpty( i );
        assert( lruList.getValue( nodes[i] ) == data.get() + i * SIZE_ELEM );
        if( i % 2 )
            lruList.elementAccessed( nodes[i] );
    }

    nodes[0] = lruList.getEmpty( NUM_ELEM + 1 );
    assert( lruList.getValue( nodes[0] ) == data.get() );

    delete[] nodes;
#endif
}
