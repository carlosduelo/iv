/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>
#include <iv/common/global.h>
#include <iv/common/mortonCodeUtil_CPU.h>

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>

#include <boost/progress.hpp>

#include <fstream>
#include <chrono>

namespace iv
{

namespace bench
{

void fileBench( int /*argc*/, char ** /*argv*/ )
{
}

}
}

int main( int argc, char ** argv )
{
    if( !iv::IV::init( argc, argv ) )
        return 0;

    iv::bench::fileBench( argc, argv );

    iv::IV::exit();

    return 0;
}
