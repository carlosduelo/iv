/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>
#include <iv/common/global.h>

namespace
{
    bool _init = false;
}

namespace iv
{

bool init( const int /*argc*/, char ** /*argv*/ )
{
    if( _init )
        return true;
    _init = true;

    return true;
}

void exit()
{
    if( !_init )
        return;
}

}

