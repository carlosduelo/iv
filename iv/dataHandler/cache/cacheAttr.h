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
        : cubeCacheImpl( IV_CUBE_CACHE_SIMPLE )
        , cubeDim( 0 )
        , cubeSize( 0 )
        , brickDim( 0 )
        , brickSize( 0 )
        , deviceID( 0 )
    {
        const Global& global = IV::getGlobal();
        cubeDim  = exp2f( global.getnLevels() - global.getCubeLevel() );
        cubeSize = powf( cubeDim + 2 * global.getCubeInc(), 3 );

        brickDim  = exp2f( global.getnLevels() - global.getBrickLevel() );
        brickSize = powf( cubeDim + 2 * global.getBrickInc(), 3 );
    }

    // Cube properties
    cube_cache_impl_t cubeCacheImpl;
    uint32_t cubeDim;   // 2^( nLevels - cubeLevel )
    uint32_t cubeSize;  // 3^( cubeDim + 2 * cubeInc )

    // Brick properties
    uint32_t brickDim;   // 2^( nLevels - brickLevel )
    uint32_t brickSize;  // 3^( brickDim + 2 * brickInc )
    int      deviceID;
};

}

}

#endif // _IV_CACHE_ATTR_H_
