/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/global.h>

namespace iv
{
    const Global& Global::getGlobal()
    {
        static Global instance;
        return instance;
    }
}
