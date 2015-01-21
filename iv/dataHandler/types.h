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

typedef unsigned                    file_type_t;
typedef std::vector< std::string >  file_args_t;

class Cache;
class CacheAttr;
class CubeCache;
class CacheObject;
class LRULinkedList;
class ObjectHandler;

typedef std::unique_ptr< Cache >            CachePtr;
typedef std::shared_ptr< CacheAttr >        CacheAttrPtr;
typedef std::unique_ptr< CubeCache >        CubeCachePtr;
typedef std::shared_ptr< CacheObject >      CacheObjectPtr;
typedef std::unique_ptr< LRULinkedList >    LRULinkedListPtr;
typedef std::shared_ptr< ObjectHandler >    ObjectHandlerPtr;

}

}

#endif // _IV_DATA_HANDLER_TYPES_H_
