/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_H_
#define _IV_OCTREE_H_

#include <iv/common/types.h>

#include <string>
#include <set>

namespace iv
{

namespace DataHandler
{

class Octree
{
public:
    Octree( const std::string& filePath )
                : _filePath( filePath )
    {
    }

    bool open();

    void close();

    level_t getLevel() const { return _level; }

    level_t getnLevels() const { return _nLevels; }

    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }

    const std::string& getFilePath() const { return _filePath; }

    uint32_t getMaxHeight() const { return _maxHeight; }

    const std::vector< index_node_t * >& getData() const { return _octree; }

    const std::vector< uint32_t >& getSizes() const { return _sizes; }

private:
    level_t _nLevels;
    level_t _level;

    std::set<float> _isosurfaces;

    const std::string _filePath;

    uint32_t      _maxHeight;

    std::vector< index_node_t* > _octree;
    std::vector< uint32_t >      _sizes;

    char *  _startFile;
    int     _fileD;
    size_t  _lengthFile;
};

}

}

#endif // _IV_OCTREE_H_
