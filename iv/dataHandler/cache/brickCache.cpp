/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/brickCache.h>

#include <iv/common/init.h>
#include <iv/common/mortonCodeUtil_CPU.h>

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
    // Get cube from cube cache
    index_node_t cubeID = _obj->getID() >> 3 * (_attr->brickLevel - _attr->cubeLevel);
    const ObjectHandlerPtr objData = _cubeCache->get( cubeID );
    const float* cubeData = objData->lock();

    if( !cubeData )
    {
        _callback( _obj, false );
        return;
    }

    //Create Cuda Stream
    cudaStream_t stream;
    if( cudaSuccess != cudaStreamCreate( &stream ) )
    {
        std::cerr << "Error creating cuda stream: "
                  << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        _callback( _obj, false );
        return;
    }
    // Copy brick
    void * ptrData = (void*)_data.get();
    if( !ptrData )
    {
        _callback( _obj, false );
        return;
    }

    if( cudaMemsetAsync( ptrData, 0, _attr->brickSize, stream ) )
    {
        std::cerr << "Error init brick: "
                  << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        _callback( _obj, false );
        return;
    }
    vec3int32_t coord = getMinBoxIndex2( _obj->getID(),
                                         _attr->brickLevel,
                                         _cubeCache->getnLevels() );
    vec3int32_t coordC = getMinBoxIndex2( cubeID,
                                          _attr->cubeLevel,
                                          _cubeCache->getnLevels() );

    std::cout << _obj->getID()<<" brick "<<coord << std::endl;
    std::cout << cubeID <<" cube "<<coordC << std::endl;
    coord -= coordC;

    const Global& global = IV::getGlobal();
    int32_t dimCube  = _attr->cubeDim + 2 * global.getCubeInc();
    int32_t dimBrick = _attr->brickDim + 2 * global.getBrickInc();

    cudaMemcpy3DParms myParms;
    myParms.srcArray = 0;
    myParms.dstArray = 0;
    myParms.srcPtr = make_cudaPitchedPtr( (void*)cubeData,
                                          dimCube*sizeof(float),
                                          dimCube,
                                          dimCube );
    myParms.dstPtr = make_cudaPitchedPtr( (void*)ptrData,
                                          dimBrick*sizeof(float),
                                          dimBrick,
                                          dimBrick );
    myParms.extent = make_cudaExtent( dimBrick*sizeof(float),
                                      dimBrick,
                                      dimBrick );
    myParms.dstPos = make_cudaPos( 0, 0, 0);
    myParms.srcPos = make_cudaPos( coord.z()*sizeof(float),
                                   coord.y(),
                                   coord.x() );
    myParms.kind = cudaMemcpyHostToDevice;

    if (cudaSuccess != cudaMemcpy3DAsync( &myParms, stream ) )
    {
        std::cerr << "Error copy cuda stream: "
                  << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        _callback( _obj, false );
        return;
    }

    if( cudaSuccess != cudaStreamSynchronize ( stream ) )
    {
        std::cerr << "Error sync cuda stream: "
                  << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        _callback( _obj, false );
        return;
    }
    if( cudaSuccess != cudaStreamDestroy( stream ) )
    {
        std::cerr << "Error destroyin cuda stream: "
                  << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        _callback( _obj, false );
        return;
    }

    _callback( _obj, true );
}

bool BrickCache::_init()
{
    // Create Cube Cache
    _cubeCache.reset( CreateCubeCache( _attr->cubeCacheImpl ) );
    if( !_cubeCache )
        return false;
    assert( _cubeCache->getnLevels() != 0 );
    if( !_cubeCache->init( _attr ) )
        return false;

    if( cudaSuccess != cudaSetDevice( _attr->deviceID ) )
    {
        std::cerr << "Error setting cuda device id " << _attr->deviceID
                  << ": " << cudaGetErrorString( cudaGetLastError() ) << std::endl;
        return false;
    }

    const Global& global = IV::getGlobal();
    // Allocate memory
    _numElements = global.getCacheSizeGPU() /
                                    ( _attr->brickSize * sizeof( float ) );
    if( _numElements == 0 )
    {
        std::cerr << "Not avaible space creating brick cache" << std::endl;
        return false;
    }

    if( cudaSuccess != cudaMalloc( &_data,
                          _numElements * _attr->brickSize *sizeof( float ) ) )
    {
        std::cerr << "Not avaible space creating brick cache" << std::endl;
        return false;
    }

    // Allocate and Copy Grids
    const vec3int32_t& dimension = _cubeCache->getRealDimension();
    uint32_t sizeGrid = dimension.x() + dimension.y() + dimension.z();
    if( cudaSuccess != cudaMalloc( &_xGrid, sizeGrid * sizeof( float ) ) )
    {
        std::cerr << "Not avaible space creating brick cache" << std::endl;
        return false;
    }
    if( cudaSuccess != cudaMemcpy( _xGrid,
                                  _cubeCache->getGridX(),
                                  dimension.x()*sizeof(float),
                                  cudaMemcpyHostToDevice)        &&
        cudaSuccess != cudaMemcpy( _xGrid + dimension.x(),
                                  _cubeCache->getGridY(),
                                  dimension.y()*sizeof(float),
                                  cudaMemcpyHostToDevice)        &&
        cudaSuccess != cudaMemcpy( _xGrid + dimension.x() + dimension.y(),
                                  _cubeCache->getGridZ(),
                                  dimension.z()*sizeof(float),
                                  cudaMemcpyHostToDevice)        )
    {
        std::cerr << "Error copying grids" << std::endl;
        return false;
    }

    _inc.set( global.getBrickInc(), global.getBrickInc(), global.getBrickInc()  );
    _dim.set( _attr->brickDim, _attr->brickDim, _attr->brickDim );

    // Start Cube Cache
    return true;
}

void BrickCache::_stop()
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( _readers.size() > 0 )
        _cond.wait( mlock );

    // Stop Cube Cache
    _cubeCache->stop();

    if( cudaSuccess != cudaFree( _data ) &&
        cudaSuccess != cudaFree( _xGrid ) )
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
                                        _attr,
                                        obj,
                                        data,
                                        std::bind( &BrickCache::_finishRead,
                                                   this, _1, _2 ) ) );
        reader->start();
    }
}

void BrickCache::_finishRead( const CacheObjectPtr& obj, const bool valid )
{
    if( valid )
        obj->setState( CacheObject::CACHED );
    else
        obj->setState( CacheObject::INVALID );

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

level_t  BrickCache::getnLevels() const
{
    return _cubeCache->getnLevels();
}

}

}
