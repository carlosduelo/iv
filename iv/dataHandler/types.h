/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_DATA_HANDLER_TYPES_H_
#define _IV_DATA_HANDLER_TYPES_H_

#include <vector>
#include <string>
#include <memory>

namespace iv
{

namespace DataHandler
{

// File types supported
#define IV_FILE_TYPE_UNKNOWN    0
#define IV_FILE_TYPE_HDF5       1
#define IV_FILE_TYPE_TEST       2

// Cube Cache Implementations
#define IV_CUBE_CACHE_SIMPLE 0

// Octree types
#define IV_OCTREE_SINGLE 0
#define IV_OCTREE_MULTI  1

typedef uint32_t                    file_type_t;
typedef std::vector< std::string >  file_args_t;

typedef uint32_t                    cache_type_t;
typedef uint32_t                    cube_cache_impl_t;
typedef uint32_t                    octree_type_t;

class Cache;
class CacheAttr;
class CubeCache;
class ControlCache;
class CacheObject;
class LRULinkedList;
class ObjectHandler;

class OctreeGenAttr;
class OctreeGen;
class OctreeConstructorAttr;
class OctreeConstructor;
class Worker;
class DataWarehouse;
class Octree;

typedef std::shared_ptr< Cache >            CachePtr;
typedef std::shared_ptr< CacheAttr >        CacheAttrPtr;
typedef std::unique_ptr< ControlCache >     ControlCachePtr;
typedef std::shared_ptr< CacheObject >      CacheObjectPtr;
typedef std::unique_ptr< LRULinkedList >    LRULinkedListPtr;
typedef std::shared_ptr< ObjectHandler >    ObjectHandlerPtr;

typedef std::shared_ptr< Octree >                   OctreePtr;
typedef std::shared_ptr< OctreeGenAttr >            OctreeGenAttrPtr;
typedef std::shared_ptr< OctreeGen >                OctreeGenPtr;
typedef std::shared_ptr< OctreeConstructorAttr >    OctreeConstructorAttrPtr;
typedef std::shared_ptr< OctreeConstructor >        OctreeConstructorPtr;
typedef std::unique_ptr< Worker >                   WorkerPtr;
typedef std::shared_ptr< DataWarehouse >            DataWarehousePtr;

}

}

#endif // _IV_DATA_HANDLER_TYPES_H_
