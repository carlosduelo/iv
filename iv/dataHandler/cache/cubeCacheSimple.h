/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CUBECACHE_H_
#define _IV_CUBECACHE_H_

#include <iv/dataHandler/cache/controlCache.h>
#include <iv/dataHandler/fileReader/fileHandler.h>

namespace iv
{

namespace DataHandler
{

class CubeCacheSimple : public ControlCache
{
public:
    // Data From Volume
    virtual const vec3int32_t& getRealDimension() const;

    virtual const float * getGridX() const;

    virtual const float * getGridY() const;

    virtual const float * getGridZ() const;
private:
    FileHandlerPtr  _file;

    bool _init();

    void _stop();

    void _readProcess( const CacheObjectPtr& obj,
                               const LRULinkedList::node_ref data );
};

}

}

#endif // _IV_CUBECACHE_H_
