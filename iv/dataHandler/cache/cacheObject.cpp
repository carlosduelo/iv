/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/cacheObject.h>

#include <iv/dataHandler/cache/objectHandler.h>

namespace iv
{

namespace DataHandler
{

CacheObject::CacheObject( const index_node_t id,
                          const CacheObjectCallback_t  lockCallback,
                          const CacheObjectCallback_t  unlockCallback )
    : _state( NO_CACHED )
    , _nRefs( 0 )
    , _id( id )
    , _lockCallback( lockCallback )
    , _unlockCallback( unlockCallback )

{}

index_node_t CacheObject::getID() const
{
    return _id;
}

void CacheObject::setData( const LRULinkedList::node_ref& data)
{
    std::unique_lock< std::mutex > mlock( _mutex );
    _data = data;
}

const float* CacheObject::try_lock()
{
    std::unique_lock< std::mutex > mlock( _mutex );

    if( !_data.isValid() || _state != CACHED )
        return 0;

    _lockCallback( this );
    return _data.get();
}

const float* CacheObject::lock()
{
    std::unique_lock< std::mutex > mlock( _mutex );

    while( !_data.isValid() && _state != CACHED )
        _condState.wait( mlock );

    _lockCallback( this );
    return _data.get();
}

void CacheObject::unlock()
{
    std::unique_lock< std::mutex > mlock( _mutex );

    _nRefs--;
    if( _nRefs == 0 )
    {
        if( _unlockCallback( this ) )
        {
            _state = NO_CACHED;
            _condRefs.notify_one();
        }
    }
}

void CacheObject::waitUnlocked()
{
    std::unique_lock< std::mutex > mlock( _mutex );
    while( _nRefs > 0 )
        _condRefs.wait( mlock );
}

CacheObject::State CacheObject::getState()
{
    std::unique_lock< std::mutex > mlock( _mutex );
    return _state;
}

void CacheObject::setState( const State& state )
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( state == CACHED )
        _condState.notify_all();

    _state = state;
}

ObjectHandlerPtr CacheObject::createHandler()
{
    std::unique_lock< std::mutex > mlock( _mutex );
    _nRefs++;
    return ObjectHandlerPtr( new ObjectHandler( shared_from_this() ) );
}

}

}
