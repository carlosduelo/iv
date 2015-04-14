/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_GEN_ATTR_H_
#define _IV_OCTREE_GEN_ATTR_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <set>

namespace iv
{

namespace DataHandler
{

class OctreeGenAttr
{
public:
    OctreeGenAttr( const octree_type_t octreeType,
                   const level_t level,
                   const level_t readLevel,
                   const level_t constructorLevel,
                   const uint32_t cubeInc,
                   const std::string& filePath,
                   const file_type_t file_type,
                   const file_args_t file_args,
                   const std::set<float>& isosurfaces )
           : _octreeType( octreeType )
           , _level( level )
           , _readLevel( readLevel )
           , _constructorLevel( constructorLevel )
           , _cubeInc( cubeInc )
           , _isosurfaces( isosurfaces )
           , _filePath( filePath )
           , _file_type( file_type )
           , _file_args( file_args )
    {
    }

    octree_type_t getOctreeType() const { return _octreeType; }

    level_t getLevel() const { return _level; }

    level_t getReadLevel() const { return _readLevel; }

    level_t getConstructorLevel() const { return _constructorLevel; }

    uint32_t getCubeInc() const { return _cubeInc; }

    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }

    const std::string& getFilePath() const { return _filePath; }

    file_type_t getFileType() const { return _file_type; }

    const file_args_t getFileArgs() const { return _file_args; }

private:
    const octree_type_t _octreeType;
    const level_t _level;
    const level_t _readLevel;
    const level_t _constructorLevel;

    const uint32_t _cubeInc;

    const std::set<float> _isosurfaces;

    const std::string _filePath;

    const file_type_t _file_type;
    const file_args_t _file_args;

};

}

}

#endif // _IV_OCTREE_GEN_ATTR_H_
