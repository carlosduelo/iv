/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/controlCache.h>

#include <iv/dataHandler/cache/cacheAttr.h>
#include <iv/dataHandler/cache/cacheObject.h>
#include <iv/dataHandler/cache/objectHandler.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>

using namespace std::placeholders;  // for _1, _2, _3...

namespace iv
{

namespace DataHandler
{

bool ControlCache::init( const CacheAttrPtr& attr )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    _attr = attr;
    if( !_init() )
        return false;

    _lruList.reset( new LRULinkedList() );
    if( !_lruList->init( _numElements, _data, _attr->cubeSize ) )
    {
        std::cerr << "Error init cube cache" << std::endl;
        return false;
    }

    _toReadThread.reset( new std::thread( &ControlCache::_updateToRead, this ) );
    if( !_toReadThread )
        return false;

    _stopped = false;
    _toRead.start();

    return true;
}

void ControlCache::stop()
{
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        if( _stopped )
            return;

        _stopped = true;

        // Wait for threads
        _toRead.stop();

        for ( auto it = _cubesTable.begin(); it != _cubesTable.end(); ++it )
            it->second->waitUnlocked( );
    }

    _cubesTable.clear();
    _toReadThread->join();
    _lruList->stop();

    _stop();
}

const ObjectHandlerPtr ControlCache::get( const index_node_t id )
{
    std::unique_lock< std::mutex > mlock( _mutex );

    if( _stopped )
        return ObjectHandlerPtr();

    CacheObjectPtr& obj = _cubesTable[ id ];
    if( !obj )
    {
        obj.reset( new CacheObject( id,
                                    std::bind( &ControlCache::_lock, this, _1 ),
                                    std::bind( &ControlCache::_unlock, this, _1 ) )
                 );
    }

    if( obj->getState() == CacheObject::NO_CACHED )
    {
        obj->setState( CacheObject::READING );
        _toRead.push( obj );
    }

    return obj->createHandler();
}

bool ControlCache::_lock( CacheObject* obj )
{
    _lruList->elementAccesed( obj->_data );
    return true;
}

bool ControlCache::_unlock( CacheObject* obj )
{
    _lruList->remove( obj->_data );
    return true;
}

void ControlCache::_updateToRead()
{
    while( 1 )
    {
        CacheObjectPtr obj;
        _toRead.pop( obj );
        if( !obj )
            return;

        LRULinkedList::node_ref cEntry( obj->getID() );
        if( _lruList->getEmptySync( cEntry ) && cEntry.isValid() )
        {
            obj->setData( cEntry );
            obj->setState( CacheObject::CACHED );
            continue;
        }

        if( !cEntry.isValid() )
            return;

        obj->setData( cEntry );
        {
            std::unique_lock< std::mutex > mlock( _mutex );
            if( _stopped )
                return;

            _readProcess( obj, cEntry );
        }
    }
}

}

}
