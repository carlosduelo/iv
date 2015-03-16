/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_ATTR_H_
#define _IV_OCTREE_CONSTRUCTOR_ATTR_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <iv/dataHandler/octree/octreeGenAttr.h>

#include <set>

namespace iv
{

namespace DataHandler
{

class OctreeConstructorAttr
{
public:
    OctreeConstructorAttr( const OctreeGenAttrPtr& attr,
                           const uint32_t numThreads,
                           const bool hyperThreading,
                           const bool cuda )
           : _octreeType( attr->getOctreeType() )
           , _hyperThreading( hyperThreading )
           , _useCuda( cuda )
           , _numThreads( numThreads )
           , _nLevels( attr->getnLevels() )
           , _level( attr->getLevel() )
           , _readLevel( attr->getReadLevel() )
           , _constructorLevel( attr->getConstructorLevel() )
           , _cubeInc( attr->getCubeInc() )
           , _isosurfaces( attr->getIsosurfaces() )
           , _offset( attr->getOffset() )
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
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_ATTR_H_
