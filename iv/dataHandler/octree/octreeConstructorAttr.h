/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_ATTR_H_
#define _IV_OCTREE_CONSTRUCTOR_ATTR_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <set>

namespace iv
{

namespace DataHandler
{

class OctreeConstructorAttr
{
public:
    OctreeConstructorAttr( const octree_type_t octreeType,
                           const uint32_t numThreads,
                           const bool hyperThreading,
                           const bool cuda,
                           const level_t nLevels,
                           const level_t level,
                           const level_t readLevel,
                           const level_t constructorLevel,
                           const uint32_t cubeInc,
                           const vec3int32_t& offset,
                           const std::string& filePath,
                           const std::set<float>& isosurfaces )
           : _octreeType( octreeType )
           , _hyperThreading( hyperThreading )
           , _useCuda( cuda )
           , _numThreads( numThreads )
           , _nLevels( nLevels )
           , _level( level )
           , _readLevel( readLevel )
           , _constructorLevel( constructorLevel )
           , _cubeInc( cubeInc )
           , _isosurfaces( isosurfaces )
           , _offset( offset )
           , _filePath( filePath )
    {
    }

    octree_type_t getOctreeType() const { return _octreeType; }

    bool useHyperThreading() const { return _hyperThreading; }

    bool useCuda() const { return _useCuda; }

    uint32_t getNumThreads() const { return _numThreads; }

    level_t getLevel() const { return _level; }

    level_t getnLevels() const { return _nLevels; }

    level_t getReadLevel() const { return _readLevel; }

    level_t getConstructorLevel() const { return _constructorLevel; }

    uint32_t getCubeInc() const { return _cubeInc; }

    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }

    const vec3int32_t& getOffset() const { return _offset; }

    const std::string& getFilePath() const { return _filePath; }

private:
    const octree_type_t _octreeType;
    const bool          _hyperThreading;
    const bool          _useCuda;
    const uint32_t      _numThreads;

    const level_t _nLevels;
    const level_t _level;
    const level_t _readLevel;
    const level_t _constructorLevel;

    const uint32_t _cubeInc;

    const std::set<float> _isosurfaces;

    const vec3int32_t _offset;

    const std::string _filePath;
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_ATTR_H_
