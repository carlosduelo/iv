/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CACHE_ATTR_H_
#define _IV_CACHE_ATTR_H_

#include <iv/common/types.h>
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
        , nLevels( 0 )
        , cubeCacheImpl( IV_CUBE_CACHE_SIMPLE )
        , cubeLevel( 0 )
        , cubeInc( 0 )
        , cubeDim( 0 )
        , cubeSize( 0 )
        , brickLevel( 0 )
        , brickInc( 0 )
        , brickDim( 0 )
        , brickSize( 0 )
        , deviceID( 0 )
    {
    }

    // File attributes
    file_type_t file_type;
    file_args_t file_args;

    level_t nLevels;

    // Cube properties
    cube_cache_impl_t cubeCacheImpl;
    level_t cubeLevel;
    uint32_t cubeInc;
    uint32_t cubeDim;   // 2^( nLevels - cubeLevel )
    uint32_t cubeSize;  // 3^( cubeDim + 2 * cubeInc )

    // Brick properties
    level_t brickLevel;
    uint32_t brickInc;
    uint32_t brickDim;   // 2^( nLevels - brickLevel )
    uint32_t brickSize;  // 3^( brickDim + 2 * brickInc )
    int      deviceID;

    bool compute()
    {
        if( brickLevel > 0 && cubeLevel > brickLevel )
        {
            std::cerr << "bricklevel may be greater than cubeLevel" << std::endl;
            return false;
        }

        cubeDim  = exp2f( nLevels - cubeLevel );
        cubeSize = powf( cubeDim + 2 * cubeInc, 3 );

        brickDim  = exp2f( nLevels - brickLevel );
        brickSize = powf( cubeDim + 2 * brickInc, 3 );

        return true;
    }

};

}

}

#endif // _IV_CACHE_ATTR_H_
