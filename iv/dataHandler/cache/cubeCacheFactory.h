/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CUBE_CACHE_FACTORY_H_
#define _IV_IV_CUBE_CACHE_FACTORY_H_

#include <iv/dataHandler/cache/cubeCacheSimple.h>

namespace iv
{

namespace DataHandler
{

ControlCache * CreateCubeCache( cube_cache_impl_t impl )
{
    switch( impl )
    {
        case IV_CUBE_CACHE_SIMPLE:
            return new CubeCacheSimple();
        default:
            return 0;
    }
}

}

}
#endif // _IV_IV_CUBE_CACHE_FACTORY_H_
