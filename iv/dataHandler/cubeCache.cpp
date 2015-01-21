/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cubeCache.h>

#include <iv/dataHandler/cacheAttr.h>
#include <iv/dataHandler/cacheObject.h>
#include <iv/dataHandler/objectHandler.h>
#include <iv/dataHandler/factoryFileHandler.h>

using namespace std::placeholders;  // for _1, _2, _3...

namespace iv
{

namespace DataHandler
{
bool CubeCache::_init()
{
    _file =  FactoryFileHandler::CreateFileHandler( _attr->file_type,
                                                    _attr->file_args );
    if( !_file )
        return false;

    // Allocate memory
    _numElements = _attr->sizeCache /
                                    ( _attr->cubeSize * sizeof( float ) );
    if( _numElements == 0 )
    {
        std::cerr << "Not avaible space creating cube cache" << std::endl;
        return false;
    }
    _data =  new float[ _numElements * _attr->cubeSize ];
    if( !_data )
    {
        std::cerr << "Not avaible space creating cube cache" << std::endl;
        return false;
    }

    _bufferPlane.reset( new float[ _attr->cubeDim + 2 * _attr->cubeInc ] );
    if( !_bufferPlane )
    {
        std::cerr << "Not avaible space creating cube cache" << std::endl;
        return false;
    }

    return true;
}

void CubeCache::_stop()
{
    _file.reset();
    _attr.reset();
    delete[] _data;
    _bufferPlane.reset();
}

void CubeCache::_readProcess( const CacheObjectPtr& obj,
                              const LRULinkedList::node_ref data )
{
    float* d = (float*)data.get();
    d[0] = obj->_id;
    obj->setState( CacheObject::CACHED );
}

}

}
