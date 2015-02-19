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

iv::DataHandler::CachePtr       _brickCache;
iv::DataHandler::CacheAttrPtr   _attr;

void test2()
{
    iv::index_node_t index = iv::coordinateToIndex( iv::vec3int32_t( 0,0,0),
                                                    _attr->cubeLevel,
                                                    _attr->nLevels );

    const iv::DataHandler::ObjectHandlerPtr obj = _brickCache->get( index );
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
                                                    _attr->nLevels );

    const iv::DataHandler::ObjectHandlerPtr obj = _brickCache->get( index );
    if( !obj )
        return;
    const float * d = obj->try_lock();
    std::cout << "-- " << d << std::endl;
    d = obj->lock();
    std::cout << d << " " << std::endl;
}

int main( int, char ** )
{
    _attr.reset( new iv::DataHandler::CacheAttr() );
    _brickCache.reset( new iv::DataHandler::Cache( IV_BRICK_CACHE ) );

    // Set attributes
    _attr->file_type = IV_FILE_TYPE_TEST;
    _attr->file_args.push_back( "128" );
    _attr->sizeCacheCPU = 100 * 1024 * 1024; // 100MB
    _attr->sizeCacheGPU = 100 * 1024 * 1024; // 100MB
    _attr->offset.set( 0, 0, 0 );
    _attr->nLevels = 7; // 2^9 = 128
    _attr->brickLevel = 4;
    _attr->brickInc = 2;
    _attr->deviceID = 0;

    // TEST 1
    {
        _brickCache->init( _attr );
        _brickCache->stop();
    }

    // TEST 2
    {
        _brickCache->init( _attr );

        std::thread _t( test2 );

        _brickCache->stop();
        _t.join();
    }


    // TEST 3
    {
        _brickCache->init( _attr );

        std::thread _t( test3 );

        _t.join();
        _brickCache->stop();
    }

    // TEST 4
    {
        _brickCache->init( _attr );
        iv::index_node_t index = iv::coordinateToIndex( iv::vec3int32_t( 0,0,0),
                                                        _attr->cubeLevel,
                                                        _attr->nLevels );

        for( unsigned i = 0; i < 2; i++ )
        {
            const iv::DataHandler::ObjectHandlerPtr obj = _brickCache->get( index );
            if( !obj )
                continue;

            const float * d = obj->lock();
            d = obj->lock();
            if( d )
                std::cout << d << " " << d[0] <<std::endl;
        }

        _brickCache->stop();
    }
}