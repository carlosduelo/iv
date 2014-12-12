/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CUBECACHE_H_
#define _IV_CUBECACHE_H_

#include <iv/dataHandler/fileHandler.h>

#include <unordered_map>

namespace iv
{

namespace DataHandler
{

struct CubeCacheAttr
{
    // File attributes
    file_type_t file_type;
    file_args_t file_args;

    // Cache attributes
    size_t       sizeCache; // Capacity in bytes
    vec3int32_t  offset;
    vec2int32_t  planeDimension;

    level_t nLevels;

    // Cube properties
    level_t cubeLevel;
    uint32_t cubeInc;
    uint32_t cubeDim; // 2^( nLevels - cubeLevel )

};

typedef std::shared_ptr< CubeCacheAttr > CubeCacheAttrPtr;

class CubeCache
{
public:
    bool init( const CubeCacheAttrPtr& attr );

    void stop();

    float * get( const index_node_t id );

    void remove( const index_node_t id );

private:
    CubeCacheAttrPtr    _attr;

    FileHandlerPtr      _file;

    float *             _data;

};

}

}

#endif // _IV_CUBECACHE_H_
