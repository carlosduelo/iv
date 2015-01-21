/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CUBECACHE_H_
#define _IV_CUBECACHE_H_

#include <iv/dataHandler/controlCache.h>
#include <iv/dataHandler/fileHandler.h>

namespace iv
{

namespace DataHandler
{

class CubeCache : public ControlCache
{
private:
    FileHandlerPtr  _file;

    std::unique_ptr< float[] >  _bufferPlane;

    bool _init();

    void _stop();

    void _readProcess( const CacheObjectPtr& obj,
                               const LRULinkedList::node_ref data );
};

}

}

#endif // _IV_CUBECACHE_H_
