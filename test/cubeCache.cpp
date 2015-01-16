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

void test1()
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

void test2()
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
    _attr->sizeCache = 100 * 1024 * 1024; // 100MB
    _attr->offset.set( 0, 0, 0 );
    _attr->nLevels = 9; // 2^9 = 512
    _attr->cubeLevel = 4;
    _attr->cubeInc = 2;
    _attr->cubeDim = exp2f( _attr->nLevels - _attr->cubeLevel );
    _attr->cubeSize = pow( _attr->cubeDim + 2 * _attr->cubeInc, 3 );

    _cubeCache.init( _attr );
    _cubeCache.stop();

    // TEST 1
    {
        _cubeCache.init( _attr );

        std::thread _t( test1 );
        for(int i=0; i<10000; i++);

        _t.join();
        _cubeCache.stop();
    }

    // TEST 2
    {
        _cubeCache.init( _attr );

        std::thread _t( test2 );

        _t.join();

        _cubeCache.stop();
    }
}
