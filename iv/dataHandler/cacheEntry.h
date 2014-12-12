/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CACHE_ENTRY_H_
#define _IV_CACHE_ENTRY_H_

#include <iv/common/types.h>

#include <memory>

namespace iv
{

namespace DataHandler
{

class CacheEntry
{
public: 
    CacheEntry( const float* e ) : _element( e ) {}
    CacheEntry( ) : _element( 0 ) {}

    bool isValid() const { return _element != 0; }

    const float* get() const { return _element; }

private:
    const float*   _element;
};

typedef std::shared_ptr< CacheEntry > CacheEntryPtr;

}

}

#endif // _IV_CACHE_ENTRY_H_
