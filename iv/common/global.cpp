/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/global.h>

namespace iv
{
Global& Global::getInstance()
{
    static Global global;
    return global;
}

}
