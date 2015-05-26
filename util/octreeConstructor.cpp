/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>

#include <iv/dataHandler/octree/octreeGen.h>

namespace iv
{

namespace util
{

void createOctree( )
{
    DataHandler::OctreeGenPtr octreeGen( new DataHandler::OctreeGen( ) );

    octreeGen->compute( );
}

}

}

int main( int argc, char ** argv )
{
    if( !iv::IV::init( argc, argv ) )
        return 0;

    iv::util::createOctree();

    iv::IV::exit();

    return 0;
}
