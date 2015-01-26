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
    Octree( const octree_type_t octreeType,
            const level_t nLevels,
            const level_t level,
            const vec3uint32_t& offset,
            const std::string& filePath,
            const std::set<float>& isosurfaces )
                : _octreeType( octreeType )
                , _nLevels( nLevels )
                , _level( level )
                , _isosurfaces( isosurfaces )
                , _offset( offset )
                , _filePath( filePath )
    {
    }

    octree_type_t getOctreeType() const { return _octreeType; }
    level_t getLevel() const { return _level; }
    level_t getnLevels() const { return _nLevels; }
    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }
    const vec3uint32_t& getOffset() const { return _offset; }
    const std::string& getFilePath() const { return _filePath; }

private:
    const octree_type_t _octreeType;

    const level_t _nLevels;
    const level_t _level;

    const std::set<float> _isosurfaces;

    const vec3uint32_t _offset;

    const std::string _filePath;
};

}

}

#endif // _IV_OCTREE_H_
