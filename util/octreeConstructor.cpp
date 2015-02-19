/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructorAttr.h>
#include <iv/dataHandler/octree/factoryOctreeConstructor.h>

namespace iv
{

namespace util
{

int createOctree( int /*argc*/, char ** /*argv*/ )
{
    unsigned octreeConstructorImpl = IV_OCTREE_CONSTRUCTOR_MEMORY; 

    DataHandler::octree_type_t octree_type = IV_OCTREE_SINGLE;

    level_t nLevels = 8;
    level_t level = 8;
    level_t readLevel = 4;

    vec3uint32_t offset( 0,0,0 );
    std::set< float > isosurfaces;
    isosurfaces.insert( 0.3 );

    std::string file_path;

    DataHandler::OctreeConstructorAttrPtr attr( new
                    DataHandler::OctreeConstructorAttr( octree_type,
                                                          nLevels,
                                                          level,
                                                          offset,
                                                          file_path,
                                                          isosurfaces ) );
    
    DataHandler::OctreeConstructorPtr oc =
            DataHandler::CreateOctreeConstructor( octreeConstructorImpl,
                                                  attr,
                                                  readLevel ); 

    DataHandler::file_args_t args;
    args.push_back("256");
    oc->start(IV_FILE_TYPE_TEST, args);

    return 0;
}

}

}

int main( int argc, char ** argv )
{
    return iv::util::createOctree( argc, argv );
}
