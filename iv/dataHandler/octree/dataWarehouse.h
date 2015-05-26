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
    DataWarehouse( const index_node_t min, const index_node_t max )
        : _max( max )
        , _min( min )
        , _maxHeight( 0 )
        , _endRange( 0 )
        , _startRange( 0 )
        , _numRanges( 0 )
    {
    }

    ~DataWarehouse();

    bool start();

    void wait();

    void pushCube( const index_node_t id );
    void updateMaxHeight( const uint32_t h );

    const std::vector< index_node_t>& getRanges() const { return _ranges; }
    uint32_t getMaxHeight() const { return _maxHeight; }
    uint32_t getNumRanges() const { return _numRanges; }

private:
    std::thread             _thread;
    Queue< index_node_t >   _queue;

    const index_node_t      _max;
    const index_node_t      _min;
    uint32_t                _maxHeight;
    std::mutex              _mutex;
    index_node_t            _endRange;
    index_node_t            _startRange;
    index_node_t            _numRanges;

    std::vector< index_node_t >                                     _indices;
    std::vector< std::unique_ptr< std::vector< index_node_t > > >   _data;

    std::vector< index_node_t > _ranges;

    void _run( );

    void _addCube( const index_node_t cube );; 

    void _sort();

    void _vectorToRanges( const std::unique_ptr< std::vector< index_node_t > >& vector );
};

}
}

#endif // _IV_OCTREE_DATAWAREHOUSE_H_

