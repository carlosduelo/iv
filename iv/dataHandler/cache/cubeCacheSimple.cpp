/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/cubeCacheSimple.h>

#include <iv/common/mortonCodeUtil_CPU.h>
#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/cache/cacheObject.h>
#include <iv/dataHandler/cache/objectHandler.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>

using namespace std::placeholders;  // for _1, _2, _3...

namespace iv
{

namespace DataHandler
{
bool CubeCacheSimple::_init()
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

    _inc.set( _attr->cubeInc, _attr->cubeInc, _attr->cubeInc );
    _dim.set( _attr->cubeDim, _attr->cubeDim, _attr->cubeDim );

    return true;
}

void CubeCacheSimple::_stop()
{
    _file.reset();
    _attr.reset();
    delete[] _data;
    _bufferPlane.reset();
}

void CubeCacheSimple::_readProcess( const CacheObjectPtr& obj,
                              const LRULinkedList::node_ref data )
{
    vec3int32_t start = getMinBoxIndex2( obj->getID(),
                                        _attr->cubeLevel,
                                        _attr->nLevels );
    vec3int32_t end = start + _dim + _inc;
    start += _inc;

    _file->read( ( float* ) data.get(), start, end );
    obj->setState( CacheObject::CACHED );
}

}

}