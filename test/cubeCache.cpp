/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/mortonCodeUtil_CPU.h>
#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/cache/cache.h>
#include <iv/dataHandler/cache/objectHandler.h>

#include <thread>

iv::DataHandler::CachePtr       _cubeCache;
iv::DataHandler::CacheAttrPtr   _attr;

void test2()
{
    iv::index_node_t index = iv::coordinateToIndex( iv::vec3int32_t( 0,0,0),
                                                    _attr->cubeLevel,
                                                    _cubeCache->getnLevels() );

    const iv::DataHandler::ObjectHandlerPtr obj = _cubeCache->get( index );
    if( !obj )
        return;
    const float * d = obj->try_lock();
    std::cout << d << std::endl;
    d = obj->lock();
    if( d )
        std::cout << d << " " << d[0] <<std::endl;
}

void test3()
{
    iv::index_node_t index = iv::coordinateToIndex( iv::vec3int32_t( 0,0,0),
                                                    _attr->cubeLevel,
                                                    _cubeCache->getnLevels() );

    const iv::DataHandler::ObjectHandlerPtr obj = _cubeCache->get( index );
    if( !obj )
        return;
    const float * d = obj->try_lock();
    std::cout << d << std::endl;
    d = obj->lock();
    if( d )
        std::cout << d << " " << d[0] << std::endl;
}

int main( int, char ** )
{
    _attr.reset( new iv::DataHandler::CacheAttr() );
    _cubeCache.reset( new iv::DataHandler::Cache( ) );

    // Set attributes
    _attr->file_type = IV_FILE_TYPE_TEST;
    _attr->file_args.push_back( "128" );
    _attr->cubeLevel = 4;
    _attr->cubeInc = 2;

    // TEST 1
    {
        _cubeCache->init( _attr );
        _cubeCache->stop();
    }

    // TEST 2
    {
        _cubeCache->init( _attr );

        std::thread _t( test2 );

        _cubeCache->stop();
        _t.join();
    }

    // TEST 3
    {
        _cubeCache->init( _attr );

        std::thread _t( test3 );

        _t.join();
        _cubeCache->stop();
    }

    // TEST 4
    {
        _cubeCache->init( _attr );
        iv::index_node_t index = iv::coordinateToIndex( iv::vec3int32_t( 0,0,0),
                                                        _attr->cubeLevel,
                                                        _cubeCache->getnLevels() );

        for( unsigned i = 0; i < 2; i++ )
        {
            const iv::DataHandler::ObjectHandlerPtr obj = _cubeCache->get( index );
            if( !obj )
                continue;

            const float * d = obj->lock();
            d = obj->lock();
            if( d )
                std::cout << d << " " << d[0] <<std::endl;
        }

        _cubeCache->stop();
    }
}
