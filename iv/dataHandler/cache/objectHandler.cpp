/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/objectHandler.h>
#include <iv/dataHandler/cache/cacheObject.h>

namespace iv
{

namespace DataHandler
{
ObjectHandler::ObjectHandler( const CacheObjectPtr& obj)
    : _obj( obj )
    , _locked( false )
    , _data( 0 )
{}

ObjectHandler::~ObjectHandler()
{
    _unlock();
}

const float* ObjectHandler::try_lock()
{
    if( _locked )
       return _data;

    _data = _obj->try_lock();
    if( _data )
        _locked= true;

    return _data;
}

const float* ObjectHandler::lock()
{
    if( _locked )
       return _data;

    _data = _obj->lock();
    if( _data )
        _locked= true;

    return _data;
}

void ObjectHandler::_unlock()
{
    _obj->unlock();
}

index_node_t ObjectHandler::getID() const
{
    return _obj->getID();
}

}

}
