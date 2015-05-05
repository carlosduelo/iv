/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_GLOBALS_H_
#define _IV_GLOBALS_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <cstdint>

#include <set>

namespace iv
{

class Global
{
friend class IV;

private:
    Global()
        : _cacheSizeCPU( 1024 ) // 1024 MB
        , _cacheSizeGPU( 128 ) // 128 MB
        , _useCuda( false )
        , _maxNumThreads( 8 )
        , _hyperThreading( false )
        , _file_type( 0 )
        , _nLevels( 0 )
        , _cubeLevel( 0 )
        , _cubeInc( 2 )
#ifdef IV_USE_CUDA
        , _brickLevel( 0 )
        , _brickInc( 2 )
#endif
        , _octreeLevel( 0 )
        , _octreeFile( "" )
        , _outputFile( "" )
    {
    }

    Global(Global const&) = delete;

    void operator=(Global const&)  = delete;

    static Global& getInstance();

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

    void setFileType( const DataHandler::file_type_t file_type ){ _file_type = file_type; }
    DataHandler::file_type_t getFileType() const { return _file_type; }

    void setFileArgs( const DataHandler::file_args_t& file_args ) { _file_args = file_args; }
    const DataHandler::file_args_t& getFileArgs() const { return _file_args; }

    void setnLevels( const level_t nLevels ) { _nLevels = nLevels; }
    level_t getnLevels() const { return _nLevels; }

    void setCubeLevel( const level_t cubeLevel ) { _cubeLevel = cubeLevel; }
    level_t getCubeLevel() const { return _cubeLevel; }

#ifdef IV_USE_CUDA
    void setBrickLevel( const level_t brickLevel ) { _brickLevel = brickLevel; }
    level_t getBrickLevel() const { return _brickLevel; }
#endif

    void     setCubeInc( const uint32_t cubeInc ) { _cubeInc = cubeInc; }
    uint32_t getCubeInc() const { return _cubeInc; }

#ifdef IV_USE_CUDA
    void     setBrickInc( const uint32_t brickInc ) { _brickInc = brickInc; }
    uint32_t getBrickInc() const { return _brickInc; }
#endif
    void setOctreeLevel( const level_t octreeLevel ) { _octreeLevel = octreeLevel; }
    level_t getOctreeLevel() const { return _octreeLevel; }

    void setOctreeFile( const std::string octreeFile ) { _octreeFile = octreeFile; }
    const std::string getOctreeFile() const { return _octreeFile; }

    void setIsosurfaces( const std::set< float >& isosurfaces ) { _isosurfaces = isosurfaces; }
    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }

    void setOutputFile( const std::string file ) { _outputFile = file; }
    const std::string getOutputFile() const { return _outputFile; }

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

    // File attributes
    DataHandler::file_type_t _file_type;
    DataHandler::file_args_t _file_args;

    // Data
    level_t      _nLevels;
    level_t      _cubeLevel;
    uint32_t     _cubeInc;
#ifdef IV_USE_CUDA
    level_t     _brickLevel;
    uint32_t    _brickInc;
#endif

    // Octree data
    level_t             _octreeLevel;
    std::string         _octreeFile;
    std::set< float >   _isosurfaces;

    // Other
    std::string         _outputFile;
};

}

#endif // _IV_GLOBALS_H_

