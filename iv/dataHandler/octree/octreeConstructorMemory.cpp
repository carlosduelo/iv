/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructorMemory.h>

namespace iv
{

namespace DataHandler
{

bool OctreeConstructorMemory::start( const file_type_t& /*file_type*/,
                                     const file_args_t& /*file_args*/ )
{
    _data.reserve( _octree->getLevel() + 1 );

    return true;
}

void OctreeConstructorMemory::stop()
{
}

}

}
