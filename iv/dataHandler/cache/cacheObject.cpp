/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/cacheObject.h>

#include <iv/dataHandler/cache/objectHandler.h>
#include <iv/dataHandler/cache/cacheStats.h>
#include <iv/common/mortonCodeUtil_CPU.h>

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
    , _miss( 0 )
    , _hits( 0 )
    , _resolveTime( new std::vector< std::chrono::nanoseconds > )
{}

CacheObject::~CacheObject( )
{
    CacheStatsPtr& stats = CacheStats::getInstance();

    stats->newStat( getIndexLevel( getID() ), getID(), _miss, _hits, _resolveTime );
}

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
    {
        _miss++;
        return 0;
    }

    _hits++;
    _lockCallback( this );
    return _data.get();
}

const float* CacheObject::lock()
{
    std::unique_lock< std::mutex > mlock( _mutex );

    bool miss = false;
    if( !_data.isValid() || _state != CACHED )
    {
        _miss++;
        miss = true;
    }
    auto startC = std::chrono::high_resolution_clock::now();

    while( !_data.isValid() && _state != CACHED  && _state != INVALID )
        _condState.wait( mlock );

    auto endC = std::chrono::high_resolution_clock::now();

    if( _state == INVALID )
        return 0;

    if( miss )
        _resolveTime->push_back( std::chrono::duration_cast<
                                                std::chrono::milliseconds>(
                                                    endC - startC ) );
    else
        _hits++;

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
    if( state == CACHED || state == INVALID )
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
