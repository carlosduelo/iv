/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_DATAWAREHOUSE_H_
#define _IV_OCTREE_DATAWAREHOUSE_H_

#include <iv/common/types.h>

#include <iv/dataHandler/util/queue.hpp>

#include <thread>
#include <fstream>
#include <unordered_map>

namespace iv
{

namespace DataHandler
{

class DataWarehouse
{
public:
    DataWarehouse( const vec3int32_t dim )
        : _maxHeight( 0 )
        , _dim( dim )
        , _numCubes( 0 )
    {
    }

    ~DataWarehouse();
    bool start();

    void write( );

    void pushCube( const uint32_t x, const uint32_t y, const uint32_t z );

private:
    uint32_t                _maxHeight;
    std::mutex              _mutex;

    const vec3int32_t           _dim;
    std::unique_ptr< char[] >   _data;
    uint64_t                    _numCubes;

#if 0
    void _sort();

    void _vectorToRanges( const std::vector< index_node_t >& vector );
#endif
};

}
}

#endif // _IV_OCTREE_DATAWAREHOUSE_H_

