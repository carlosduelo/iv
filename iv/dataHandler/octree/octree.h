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

    octree_type_t getOctreeType() const { return _octreeType; }

    level_t getLevel() const { return _level; }

    level_t getnLevels() const { return _nLevels; }

    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }

    const vec3uint32_t& getOffset() const { return _offset; }

    const std::string& getFilePath() const { return _filePath; }

    uint32_t getMaxHeght() const { return _maxHeight; }

private:
    octree_type_t _octreeType;

    level_t _nLevels;
    level_t _level;

    std::set<float> _isosurfaces;

    vec3uint32_t _offset;

    const std::string _filePath;

    uint32_t      _maxHeight;
};

}

}

#endif // _IV_OCTREE_H_
