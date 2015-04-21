/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CACHE_ATTR_H_
#define _IV_CACHE_ATTR_H_

#include <iv/common/types.h>
#include <iv/common/init.h>

#include <iv/dataHandler/types.h>

#include <math.h>

namespace iv
{

namespace DataHandler
{

class CacheAttr
{
public:
    CacheAttr()
        : file_type( IV_FILE_TYPE_UNKNOWN )
        , cubeCacheImpl( IV_CUBE_CACHE_SIMPLE )
        , cubeLevel( 0 )
        , cubeDim( 0 )
        , cubeSize( 0 )
        , brickLevel( 0 )
        , brickDim( 0 )
        , brickSize( 0 )
        , deviceID( 0 )
    {
    }

    // File attributes
    file_type_t file_type;
    file_args_t file_args;

    // Cube properties
    cube_cache_impl_t cubeCacheImpl;
    level_t cubeLevel;
    uint32_t cubeDim;   // 2^( nLevels - cubeLevel )
    uint32_t cubeSize;  // 3^( cubeDim + 2 * cubeInc )

    // Brick properties
    level_t brickLevel;
    uint32_t brickDim;   // 2^( nLevels - brickLevel )
    uint32_t brickSize;  // 3^( brickDim + 2 * brickInc )
    int      deviceID;

    bool compute( level_t nLevels )
    {
        if( brickLevel > 0 && cubeLevel > brickLevel )
        {
            std::cerr << "bricklevel may be greater than cubeLevel" << std::endl;
            return false;
        }

        const Global& global = IV::getGlobal();
        cubeDim  = exp2f( nLevels - cubeLevel );
        cubeSize = powf( cubeDim + 2 * global.getCubeInc(), 3 );

        brickDim  = exp2f( nLevels - brickLevel );
        brickSize = powf( cubeDim + 2 * global.getBrickInc(), 3 );

        return true;
    }

};

}

}

#endif // _IV_CACHE_ATTR_H_
