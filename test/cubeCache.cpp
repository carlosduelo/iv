/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cubeCache.h>
#include <iv/dataHandler/cacheAttr.h>
#include <iv/dataHandler/objectHandler.h>

#include <thread>

iv::DataHandler::CubeCache      _cubeCache;
iv::DataHandler::CacheAttrPtr   _attr;

void test2()
{
    const iv::DataHandler::ObjectHandlerPtr obj = _cubeCache.get( 2 );
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
    const iv::DataHandler::ObjectHandlerPtr obj = _cubeCache.get( 2 );
    if( !obj )
        return;
    const float * d = obj->try_lock();
    std::cout << d << std::endl;
    d = obj->lock();
    std::cout << d << " " << d[0] << std::endl;
}

int main( int, char ** )
{
    _attr.reset( new iv::DataHandler::CacheAttr() );

    // Set attributes
    _attr->file_type = IV_FILE_TYPE_TEST;
    _attr->file_args.push_back( "100" );
    _attr->sizeCache = 100 * 1024 * 1024; // 100MB
    _attr->offset.set( 0, 0, 0 );
    _attr->nLevels = 9; // 2^9 = 512
    _attr->cubeLevel = 4;
    _attr->cubeInc = 2;
    _attr->compute();

    // TEST 1
    {
        _cubeCache.init( _attr );
        _cubeCache.stop();
    }

    // TEST 2
    {
        _cubeCache.init( _attr );

        std::thread _t( test2 );

        _cubeCache.stop();
        _t.join();
    }

    // TEST 3
    {
        _cubeCache.init( _attr );

        std::thread _t( test3 );

        _t.join();
        _cubeCache.stop();
    }

    // TEST 4
    {
        _cubeCache.init( _attr );

        for( unsigned i = 0; i < 2; i++ )
        {
            const iv::DataHandler::ObjectHandlerPtr obj = _cubeCache.get( 3 );
            if( !obj )
                continue;

            const float * d = obj->lock();
            d = obj->lock();
            if( d )
                std::cout << d << " " << d[0] <<std::endl;
        }

        _cubeCache.stop();
    }
}
