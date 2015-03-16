/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructorAttr.h>
#include <iv/dataHandler/octree/octreeConstructorStats.h>
#include <iv/dataHandler/octree/octreeConstructor.h>

namespace iv
{

namespace util
{

int createOctree( int argc, char ** argv )
{
    uint32_t numThreads = 8;

    DataHandler::octree_type_t octree_type = IV_OCTREE_SINGLE;

    level_t nLevels = 10;
    level_t level = 10;
    level_t readLevel = 3;
    level_t constructorLevel = 0;
    uint32_t cubeInc = 2;
    index_node_t cube = 1;

    vec3uint32_t offset( 0,0,0 );
    std::set< float > isosurfaces;
    isosurfaces.insert( 0.5 );

    std::string file_path;

    DataHandler::OctreeGenAttrPtr attrGen( new
                    DataHandler::OctreeGenAttr( octree_type,
                                                nLevels,
                                                level,
                                                readLevel,
                                                constructorLevel,
                                                cubeInc,
                                                offset,
                                                file_path,
                                                isosurfaces ) );
    DataHandler::OctreeConstructorAttrPtr attr( new
                    DataHandler::OctreeConstructorAttr( attrGen,
                                                        numThreads,
                                                        false,
                                                        false ) );

    DataHandler::OctreeConstructorPtr oc( new
                    DataHandler::OctreeConstructor( attr, cube ) );

    DataHandler::file_args_t args;
    if( argc > 1 )
    {
        args.push_back( argv[1] );
        args.push_back( argv[2] );
        oc->compute( IV_FILE_TYPE_HDF5, args );
    }
    else
    {
        args.push_back("256");
        oc->compute(IV_FILE_TYPE_TEST, args);
    }

    const DataHandler::OctreeConstructorStats& stats = oc->getStats();
    stats.print();

    return 0;
}

}

}

int main( int argc, char ** argv )
{
    return iv::util::createOctree( argc, argv );
}
