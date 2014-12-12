/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cubeCache.h>

#include <iv/dataHandler/factoryFileHandler.h>

namespace iv
{

namespace DataHandler
{

bool CubeCache::init( const CubeCacheAttrPtr& attr )
{
    _attr = attr;

    _file =  FactoryFileHandler::CreateFileHandler( _attr->file_type,
                                                    _attr->file_args );
    if( !_file )
        return false;

    return true;
}

void CubeCache::stop()
{
    _file.reset();
}

}

}
