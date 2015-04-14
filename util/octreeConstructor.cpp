/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>
#include <iv/dataHandler/octree/octreeGenAttr.h>
#include <iv/dataHandler/octree/octreeGen.h>

namespace iv
{

namespace util
{

void createOctree( int argc, char ** argv )
{
    DataHandler::octree_type_t octree_type = IV_OCTREE_SINGLE;
    level_t level = 7;
    level_t readLevel = 7;
    level_t constructorLevel = 0;
    uint32_t cubeInc = 2;
    std::vector< index_node_t > cubes;
    cubes.push_back( 1 );

    std::set< float > isosurfaces;
    isosurfaces.insert( 0.5 );

    std::string file_path = "octree";
    DataHandler::file_type_t file_type;
    DataHandler::file_args_t file_args;
    if( argc > 1 )
    {
        file_type = IV_FILE_TYPE_HDF5;
        file_args.push_back( argv[1] );
        file_args.push_back( argv[2] );
    }
    else
    {
        file_type = IV_FILE_TYPE_TEST;
        file_args.push_back("1024");
    }

    DataHandler::OctreeGenAttrPtr attr( new
                    DataHandler::OctreeGenAttr( octree_type,
                                                level,
                                                readLevel,
                                                constructorLevel,
                                                cubeInc,
                                                file_path,
                                                file_type,
                                                file_args,
                                                isosurfaces ) );

    DataHandler::OctreeGenPtr octreeGen( new DataHandler::OctreeGen( attr ) );

    octreeGen->compute( cubes );
    octreeGen->printStats();
}

}

}

int main( int argc, char ** argv )
{
    if( !iv::init( argc, argv ) )
        return 0;

    iv::util::createOctree( argc, argv );

    iv::exit();

    return 0;
}
