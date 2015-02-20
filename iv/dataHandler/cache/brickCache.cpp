/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/brickCache.h>

#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/cache/cubeCacheFactory.h>
#include <iv/dataHandler/cache/cacheObject.h>
#include <iv/dataHandler/cache/objectHandler.h>

#include <cuda.h>
#include <cuda_runtime.h>

using namespace std::placeholders;  // for _1, _2, _3...

namespace iv
{

namespace DataHandler
{

void Reader::start()
{
    _reader = std::thread( &Reader::_read, this );
    _reader.detach();
}

void Reader::_read()
{
    const ObjectHandlerPtr objData = _cubeCache->get( _obj->getID() );
    objData->lock();

    _callback( _obj );
}

bool BrickCache::_init()
{
    // Create Cube Cache
    _cubeCache.reset( CreateCubeCache( _attr->cubeCacheImpl ) );
    if( !_cubeCache )
        return false;

    if( cudaSuccess != cudaSetDevice( _attr->deviceID ) )
    {
        std::cerr << "Error setting cuda device id " << _attr->deviceID
                  << ": " << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        return false;
    }

    // Allocate memory
    _numElements = _attr->sizeCacheGPU /
                                    ( _attr->brickSize * sizeof( float ) );
    if( _numElements == 0 )
    {
        std::cerr << "Not avaible space creating brick cache" << std::endl;
        return false;
    }
    _data =  new float[ _numElements * _attr->brickSize ];

    if( cudaSuccess != cudaMalloc( &_data,
                          _numElements * _attr->brickSize *sizeof( float ) ) )
    {
        std::cerr << "Not avaible space creating brick cache" << std::endl;
        return false;
    }

    _inc.set( _attr->brickInc, _attr->brickInc, _attr->brickInc );
    _dim.set( _attr->brickDim, _attr->brickDim, _attr->brickDim );

    // Start Cube Cache
    return _cubeCache->init( _attr );
}

void BrickCache::_stop()
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( _readers.size() > 0 )
        _cond.wait( mlock );

    // Stop Cube Cache
    _cubeCache->stop();

    if( cudaSuccess != cudaFree( _data ) )
    {
        std::cerr << "Error free cuda memory" << std::endl;
    }
}

void BrickCache::_readProcess( const CacheObjectPtr& obj,
                           const LRULinkedList::node_ref data )
{
    std::unique_lock< std::mutex > mlock( _mutex );
    ReaderPtr& reader = _readers[ obj->getID() ];

    if( !reader )
    {
        reader = ReaderPtr( new Reader( _cubeCache,
                                        obj,
                                        data,
                                        std::bind( &BrickCache::_finishRead,
                                                   this, _1 ) ) );
        reader->start();
    }
}

void BrickCache::_finishRead( const CacheObjectPtr& obj )
{
    obj->setState( CacheObject::CACHED );

    std::unique_lock< std::mutex > mlock( _mutex );
    auto e = _readers.find( obj->getID() );

    assert( e != _readers.end() );

    _readers.erase( e );

    if( _readers.size() == 0 )
        _cond.notify_one();
}

const vec3int32_t& BrickCache::getRealDimension() const
{
    return _cubeCache->getRealDimension();
}

const float * BrickCache::getGridX() const
{
    return _cubeCache->getGridX();
}

const float * BrickCache::getGridY() const
{
    return _cubeCache->getGridY();
}

const float * BrickCache::getGridZ() const
{
    return _cubeCache->getGridZ();
}

}

}
