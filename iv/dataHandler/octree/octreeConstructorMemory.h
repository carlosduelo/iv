/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_MEMORY_H_
#define _IV_OCTREE_CONSTRUCTOR_MEMORY_H_

#include <iv/dataHandler/octree/octreeConstructor.h>

namespace iv
{

namespace DataHandler
{

class OctreeConstructorMemory : public OctreeConstructor
{
public:
    virtual bool start( const file_type_t& file_type,
                        const file_args_t& file_args);

    virtual void stop();

private:
    std::vector< std::vector< index_node_t > >  _data;

};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_MEMORY_H_
