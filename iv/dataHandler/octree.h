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
    Octree( const level_t nLevels,
            const level_t level,
            const std::string& filePath,
            const std::set<float>& isosurfaces )
                : _nLevels( nLevels )
                , _level( level )
                , _isosurfaces( isosurfaces )
                , _filePath( filePath )
    {
    }

    level_t getLevel() const { return _level; }
    level_t getnLevels() const { return _nLevels; }
    const std::set< float >& getIsosurfaces() const { return _isosurfaces; }
    const std::string& getFilePath() const { return _filePath; }

private:
    const level_t _nLevels;
    const level_t _level;

    const std::set<float> _isosurfaces;

    const std::string _filePath;
};

}

}

#endif // _IV_OCTREE_H_
