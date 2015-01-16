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
    // File attributes
    file_type_t file_type;
    file_args_t file_args;

    // Cache attributes
    size_t       sizeCache; // Capacity in bytes
    vec3int32_t  offset;

    level_t nLevels;

    // Cube properties
    level_t cubeLevel;
    uint32_t cubeInc;
    uint32_t cubeDim;   // 2^( nLevels - cubeLevel )
    uint32_t cubeSize;  // 3^( cubeDim + 2 * cubeInc )

    void compute()
    {
        cubeDim  = exp2f( nLevels - cubeLevel );
        cubeSize = powf( cubeDim + 2 * cubeInc, 3 );
    }

};

}

}

#endif // _IV_CACHE_ATTR_H_
