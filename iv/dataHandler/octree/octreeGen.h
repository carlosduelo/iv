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
    OctreeGen( const OctreeGenAttrPtr& attr )
        : _attr( attr )
    {
    }

    bool compute( std::vector< index_node_t >& cubes );

    void save();

private:
    const OctreeGenAttrPtr _attr;

    std::vector< index_node_t > _cubes;

};

}

}

#endif // _IV_OCTREE_GEN_H_

