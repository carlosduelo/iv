/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructor.h>

#include <algorithm>

#define MAX_ELEM 12 * 1024 * 1024 // ~100MB

namespace iv
{

namespace DataHandler
{

bool DataWarehouse::start()
{
    // Create Files
    index_node_t current = _min;
    while( current < _max )
    {
        _indices.push_back( current );
        _dimensions.push_back( 0 );

        std::ostringstream convert;
        convert << rand() << current << ".tmp";
        _nameTmpFiles.push_back( convert.str() );
        std::shared_ptr< std::ofstream > file;
        file.reset( new std::ofstream( convert.str().c_str(),
                                            std::ofstream::binary |
                                            std::ofstream::trunc ) );
        _tmpFiles.push_back( file );

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

    // Close Files
    for( auto f = _tmpFiles.begin(); f != _tmpFiles.end(); f++ )
        (*f)->close();

    // Sort
    _sort();

    // Remove Files
    for( auto f = _nameTmpFiles.begin(); f != _nameTmpFiles.end(); f++ )
        remove( (*f).c_str() );
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
            _tmpFiles[i]->write( (char*) &id, sizeof( index_node_t ) );
            _dimensions[i] += 1;
            return;
        }
    }

    _tmpFiles[ _indices.size() - 1 ]->write( (char*) &id, sizeof( index_node_t ) );
    _dimensions[ _indices.size() - 1 ] += 1;
}

void DataWarehouse::_fileToVector( std::ifstream& file,
                                    std::vector< index_node_t >& vector,
                                    const uint32_t dim )
{
    file.read( (char*) vector.data(), dim * sizeof( index_node_t ) );
}

void DataWarehouse::_sortVector( std::vector< index_node_t >& vector )
{
    std::sort( vector.begin(), vector.end() );
}

void DataWarehouse::_vectorToFile( std::ofstream&   file,
                                   const std::vector< index_node_t >& vector,
                                   const uint32_t dim )
{
    if( _startRange == 0 )
    {
        _endRange = vector[0];
        _startRange = vector[0];
    }

    for( uint32_t i = 1; i < dim; i++ )
    {
        if( vector[i] != _endRange + 1 )
        {
            file.write( (char*) &_startRange, sizeof( index_node_t ) );
            file.write( (char*) &_endRange, sizeof( index_node_t ) );
            //std::cout << _startRange << " " << _endRange << std::endl;
            assert( _startRange != 0 && _endRange != 0 );
            _startRange = vector[i];
        }
        _endRange = vector[i];
    }
}

void DataWarehouse::_sort()
{
    // Open End File
    std::ostringstream convert;
    convert << rand() << "Final.tmp";
    _nameEndFile = convert.str();
    std::ofstream   endFile;
    endFile.open( convert.str().c_str(),
                        std::ofstream::binary |
                        std::ofstream::trunc );

    std::vector< index_node_t >     buffer;
    buffer.reserve( MAX_ELEM );
    for( uint32_t i = 0; i < _nameTmpFiles.size(); i++ )
    {
        if( _dimensions[i] == 0 )
            continue;

        std::ifstream file( _nameTmpFiles[i].c_str(), std::ofstream::binary );
        _fileToVector( file, buffer, _dimensions[i] );
        _sortVector( buffer );
        _vectorToFile( endFile, buffer, _dimensions[i] );
        file.close();
    }
    endFile.close();
}

}

}

