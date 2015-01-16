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

bool CubeCache::init( const CacheAttrPtr& attr )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    _attr = attr;

#if 0
    _file =  FactoryFileHandler::CreateFileHandler( _attr->file_type,
                                                    _attr->file_args );
    if( !_file )
        return false;
#endif

    // Allocate memory
    const uint32_t numElements = _attr->sizeCache /
                                    ( _attr->cubeSize * sizeof( float ) );
    assert( numElements );
    _data.reset( new float[ numElements * _attr->cubeSize ] );
    assert( _data );

    _bufferPlane.reset( new float[ _attr->cubeDim + 2 * _attr->cubeInc ] );
    assert( _bufferPlane );

    _lruList.reset( new LRULinkedList() );
    if( !_lruList->init( numElements, _data.get(), _attr->cubeSize ) )
    {
        std::cerr << "Error init cube cache" << std::endl;
        return false;
    }

    _toReadThread.reset( new std::thread( &CubeCache::_updateToRead, this ) );
    if( !_toReadThread )
        return false;

    _stopped = false;
    _toRead.start();

    return true;
}

void CubeCache::stop()
{
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        _stopped = true;

        // Wait for threads
        _toRead.stop();

        _file.reset();
        _attr.reset();
        _data.reset();
        _bufferPlane.reset();

        for ( auto it = _cubesTable.begin(); it != _cubesTable.end(); ++it )
            it->second->waitUnlocked( );
    }

    _toReadThread->join();
    _lruList->stop();
}

const ObjectHandlerPtr CubeCache::get( const index_node_t id )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    if( _stopped )
        return ObjectHandlerPtr();

    CacheObjectPtr& obj = _cubesTable[ id ];
    if( !obj )
    {
        obj.reset( new CacheObject( id,
                                    std::bind( &CubeCache::_lock, this, _1 ),
                                    std::bind( &CubeCache::_unlock, this, _1 ) )
                 );
    }

    if( obj->getState() == CacheObject::NO_CACHED )
    {
        obj->setState( CacheObject::READING );
        _toRead.push( obj );
    }

    return obj->createHandler();
}

bool CubeCache::_lock( CacheObject* obj )
{
    _lruList->elementAccesed( obj->_data );
    return true;
}

bool CubeCache::_unlock( CacheObject* )
{
//    _lruList->remove( obj->_cacheEntry );
return true;
}

void CubeCache::_updateToRead()
{
    while( 1 )
    {
        LRULinkedList::node_ref cEntry;
        _lruList->getEmptySync( cEntry );

        if( !cEntry.isValid() )
            return;

        CacheObjectPtr obj;
        _toRead.pop( obj );
        if( !obj )
            return;
        {

            std::unique_lock< std::mutex > mlock( _mutex );
            if( _stopped )
                return;

            std::cout << "===" <<cEntry.getID()<<" "<<obj->_id<<std::endl;

            obj->setData( cEntry );

            if( cEntry.getID() == obj->_id )
            {
                obj->setState( CacheObject::CACHED );
                continue;
            }

            float* d = (float*)cEntry.get();
            d[0] = obj->_id;
            obj->setState( CacheObject::CACHED );
        }
    }
}

void CubeCache::_readProcess()
{
}

}

}
