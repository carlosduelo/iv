/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octreeConstructorMemory.h>

#include <iv/dataHandler/factoryFileHandler.h>

namespace iv
{

namespace DataHandler
{

bool OctreeConstructorMemory::start( const file_type_t& file_type,
                                     const file_args_t& file_args )
{
    FileHandlerPtr file =  FactoryFileHandler::CreateFileHandler( file_type,
                                                                   file_args );
    if( !file )
        return false;

    _data.reserve( _octree->getLevel() + 1 );

    return true;
}

void OctreeConstructorMemory::stop()
{
}

}

}
