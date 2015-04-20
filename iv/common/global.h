/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_GLOBALS_H_
#define _IV_GLOBALS_H_

#include <cstdint>

namespace iv
{

class Global
{
public:
    static const Global& getGlobal();

private:
    Global()
        : _cacheSizeCPU( 1024 * 1024 * 1024 ) // 256 MB
        , _cacheSizeGPU( 128 * 1024 * 1024 ) // 128 MB
        , _useCuda( false )
        , _maxNumThreads( 8 )
        , _hyperThreading( false )
        , _cubeInc( 2 )
        , _brickInc( 2 )
    {
    }

    Global(Global const&) = delete;

    void operator=(Global const&)  = delete;

public:
    void setCacheSizeCPU( const uint32_t size ){ _cacheSizeCPU = size; }
    uint32_t getCacheSizeCPU() const { return _cacheSizeCPU; }

#ifdef IV_USE_CUDA
    void setCacheSizeGPU( const uint32_t size ){ _cacheSizeGPU = size; }
    uint32_t getCacheSizeGPU() const { return _cacheSizeGPU; }

    void setUseCuda(){ _useCuda = true; }
#endif
    bool useCuda() const { return _useCuda; }

    void setMaxNumThreads( const uint32_t threads ){ _maxNumThreads = threads; }
    uint32_t getMaxNumThreads() const { return _maxNumThreads; }

    void setHyperThreading() { _hyperThreading = true; }
    bool useHyperThreading() const { return _hyperThreading; }

    void     setCubeInc( const uint32_t cubeInc ) { _cubeInc = cubeInc; }
    uint32_t getCubeInc() const { return _cubeInc; }

    void     setBrickInc( const uint32_t brickInc ) { _brickInc = brickInc; }
    uint32_t getBrickInc() const { return _brickInc; }

private:
    // Maximum memory allocated on the CPU
    uint32_t    _cacheSizeCPU;
#ifdef IV_USE_CUDA
    // Maximum memory allocated on the GPU
    uint32_t    _cacheSizeGPU;
#endif
    bool        _useCuda;

    uint32_t    _maxNumThreads;
    bool        _hyperThreading;

    // Data
    uint32_t     _cubeInc;
#ifdef IV_USE_CUDA
    uint32_t    _brickInc;
#endif
};

}

#endif // _IV_GLOBALS_H_

