/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/dataWarehouse.h>

#include <iv/common/init.h>

#include <algorithm>

namespace iv
{

namespace DataHandler
{


DataWarehouse::~DataWarehouse()
{
}

bool DataWarehouse::start()
{
    uint64_t n = ( uint64_t ) _dim.x() *
                 ( uint64_t ) _dim.y() *
                 ( uint64_t ) _dim.z();

    if( n % 8 > 0 )
        n = ( n / 8 ) + 1;
    else
        n = n / 8;

    _data.reset( new char[ n ] );
    return true;
}

void DataWarehouse::write( )
{
    const Global& global = IV::getGlobal();

    std::ofstream file( global.getOctreeFile().c_str(), std::ofstream::binary );

    level_t nLevels = global.getnLevels();
    level_t level = global.getOctreeLevel();
    const uint32_t numIsos = global.getIsosurfaces().size() ;
    file.write( (char*)&nLevels, sizeof( nLevels ) );
    file.write( (char*)&level, sizeof( level ) );
    file.write( (char*)&_maxHeight, sizeof( _maxHeight ) );
    file.write( (char*)&numIsos, sizeof( numIsos ) );
    for( auto i = global.getIsosurfaces().begin();
              i != global.getIsosurfaces().end();
              ++i )
    {
        const float iso = *i;
        file.write( (char*)&iso, sizeof( iso ) );
    }

    file.close();
    // Sort
//    _sort();
    
}

void DataWarehouse::pushCube( const uint32_t x, const uint32_t y, const uint32_t z )
{
    std::unique_lock< std::mutex > rlock( _mutex );

    if( y > _maxHeight )
        _maxHeight = y;

    uint64_t byte = ( uint64_t ) ( x * _dim.y() * _dim.z() ) +
                    ( uint64_t ) ( y * _dim.z() ) +
                    ( uint64_t ) z;

    uint32_t bit = byte % 8;
    byte = byte / 8;

    _data[ byte ] = _data[ byte ] | ( (char)0x1 << bit );
}

#if 0
void DataWarehouse::_vectorToRanges( const std::vector< index_node_t >& vector )
{
    if( vector.size() == 0 )
            return;

    index_node_t endRange = vector[0];
    index_node_t startRange = vector[0];

    for( uint32_t i = 1; i < vector.size(); i++ )
    {
        if( vector[i] != endRange + 1 )
        {
            _ranges.push_back( startRange );
            _ranges.push_back( endRange );
            _numRanges++;
            assert( startRange != 0 && endRange != 0 );
            startRange = vector[i];
        }
        endRange = vector[i];
    }
}

void DataWarehouse::_sort()
{
    // Sort all vectors
    std::cout << "Sorting vectors" << std::endl;
    std::sort( _data.begin(), _data.end() );
    _vectorToRanges( _data );
}
#endif

}

}

