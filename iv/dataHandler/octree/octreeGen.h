/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_GEN_H_
#define _IV_OCTREE_GEN_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

namespace iv
{

namespace DataHandler
{

class OctreeGen
{
public:
    OctreeGen( )
    {
    }

    bool compute( );
};

}

}

#endif // _IV_OCTREE_GEN_H_

