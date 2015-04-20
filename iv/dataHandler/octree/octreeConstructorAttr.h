/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_ATTR_H_
#define _IV_OCTREE_CONSTRUCTOR_ATTR_H_

#include <iv/dataHandler/octree/octreeGenAttr.h>

#include <set>

namespace iv
{

namespace DataHandler
{

class OctreeConstructorAttr
{
public:
    OctreeConstructorAttr( const OctreeGenAttrPtr& attr )
           : _octreeType( attr->getOctreeType() )
           , _level( attr->getLevel() )
           , _readLevel( attr->getReadLevel() )
           , _constructorLevel( attr->getConstructorLevel() )
           , _isosurfaces( attr->getIsosurfaces() )
           , _file_type( attr->getFileType() )
           , _file_args( attr->getFileArgs() )
    {
    }

    octree_type_t getOctreeType() const { return _octreeType; }

    level_t getLevel() const { return _level; }

    level_t getReadLevel() const { return _readLevel; }

    level_t getConstructorLevel() const { return _constructorLevel; }

    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }

    file_type_t getFileType() const { return _file_type; }

    const file_args_t getFileArgs() const { return _file_args; }


private:
    const octree_type_t _octreeType;

    const level_t _level;
    const level_t _readLevel;
    const level_t _constructorLevel;

    const std::set<float> _isosurfaces;

    const file_type_t _file_type;
    const file_args_t _file_args;
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_ATTR_H_
