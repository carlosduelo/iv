/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/cubeCacheFactory.h>
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
