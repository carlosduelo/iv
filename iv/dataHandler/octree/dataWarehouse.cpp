/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/dataWarehouse.h>

#include <algorithm>

#define MAX_ELEM 12 * 1024 * 1024 // ~100MB

namespace iv
{

namespace DataHandler
{


DataWarehouse::~DataWarehouse()
{
}

bool DataWarehouse::start()
{
    // Create Files
    index_node_t current = _min;
    while( current <= _max )
    {
        _indices.push_back( current );
        std::unique_ptr< std::vector< index_node_t > > ptr(
                                        new std::vector< index_node_t > );
        _data.push_back( std::move( ptr )  );

        current += MAX_ELEM; // ~100 MB
    }

    _queue.start();
    _thread = std::thread( &DataWarehouse::_run, this );
    return true;
}

void DataWarehouse::wait()
{
    _queue.push(0);
    _queue.stop();
    if( _thread.joinable() )
        _thread.join();

    // Sort
    _sort();
}

void DataWarehouse::pushCube( const index_node_t id )
{
    _queue.push( id );
}

void DataWarehouse::updateMaxHeight( const uint32_t m )
{
    std::unique_lock< std::mutex > mlock( _mutex );
    if( m > _maxHeight )
        _maxHeight = m;
}

void DataWarehouse::_run()
{
    while( 1 )
    {
        index_node_t id = 0;
        _queue.pop( id );
        if( !id )
            return;
        _addCube( id );
    }
}

void DataWarehouse::_addCube( const index_node_t id )
{
    assert( id );
    if( id < _min || id > _max )
    {
        std::cerr << "Warning! trying to push a cube not in range" << std::endl;
        return;
    }

    for( uint32_t i = 0; i < _indices.size() - 1; i++ )
    {
        if( id >= _indices[i] && id < _indices[i+1] )
        {
            _data[i]->push_back( id );
            return;
        }
    }

    _data[ _indices.size() - 1 ]->push_back( id );
}

void DataWarehouse::_vectorToRanges( const std::unique_ptr< std::vector< index_node_t > >& vector )
{
    if( vector->size() == 0 )
            return;

    if( _startRange == 0 )
    {
        _endRange = (*vector)[0];
        _startRange = (*vector)[0];
    }

    for( uint32_t i = 1; i < vector->size(); i++ )
    {
        if( (*vector)[i] != _endRange + 1 )
        {
            _ranges.push_back( _startRange );
            _ranges.push_back( _endRange );
            _numRanges++;
            assert( _startRange != 0 && _endRange != 0 );
            _startRange = (*vector)[i];
        }
        _endRange = (*vector)[i];
    }
}

void DataWarehouse::_sort()
{

    // Sort all vectors
    std::cout << "Sorting vectors" << std::endl;
    #pragma omp parallel for
    for( size_t i = 0; i < _data.size(); i++ )
        std::sort( _data[i]->begin(), _data[i]->end() );

    for( size_t i = 0; i < _data.size(); i++ )
    {
        std::unique_ptr< std::vector< index_node_t > > ptr = std::move( _data[i] );
        _vectorToRanges( ptr );
    }
}

}

}

