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
    level_t constructorLevel = 0;
    std::vector< index_node_t > cubes;
    cubes.push_back( 1 );

    DataHandler::OctreeGenPtr octreeGen( new DataHandler::OctreeGen( constructorLevel ) );

    if( octreeGen->compute( cubes ) )
        octreeGen->printStats();
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
