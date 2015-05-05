/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octree.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace iv
{
namespace DataHandler
{

bool Octree::open()
{
    struct stat sb;
    _fileD = ::open( _filePath.c_str(), O_RDONLY );
    if( _fileD == -1 )
    {
        ::perror("open");
        return false;
    }

    if( ::fstat( _fileD, &sb ) == -1 )
    {
        perror("fstat");
        return false;
    }
    _lengthFile = sb.st_size;

    _startFile = (char*) ::mmap(NULL, _lengthFile, PROT_READ, MAP_PRIVATE, _fileD, 0);
    if( _startFile == MAP_FAILED )
    {
        ::perror("mmap");
        return false;
    }

    char * data = _startFile;
    _nLevels = *( (level_t*) data );
    data += sizeof( level_t );
    _level   = *( (level_t*) data );
    data += sizeof( level_t );
    _maxHeight = *( (uint32_t*) data );
    data += sizeof( uint32_t );
    const uint32_t nIsos = *( (uint32_t*) data );
    data += sizeof( uint32_t );
    for( uint32_t i = 0; i < nIsos; i++ )
    {
        const float iso = *( (float*) data );
        data += sizeof( float );
        _isosurfaces.insert( iso );
    }
    _octree.reserve( _nLevels + 1 );
    _sizes.reserve( _nLevels + 1 );
    for( int32_t l = _nLevels; l >= 0; l-- )
    {
        const uint32_t nR = *( (uint32_t*) data );
        data += sizeof( uint32_t );
        _sizes[ l ] = nR * 2; 
        _octree[ l ] = (index_node_t*) data;
        data += _sizes[ l ] * sizeof( index_node_t );
    }

    return true;
}

void Octree::close()
{
    ::munmap( _startFile, _lengthFile );
    ::close( _fileD );
}

}
}
