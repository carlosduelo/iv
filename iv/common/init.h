/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_H_
#define _IV_H_

#include <iv/common/global.h>

namespace iv
{

class IV
{
public:
    static bool init( const int argc, char ** argv );

    static const Global& getGlobal();

    static void exit();
};

}

#endif // _IV_H_


