/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>
#include <iv/dataHandler/octree/octree.h>

namespace iv
{
namespace util
{

void octreeStats()
{
    const Global& global = IV::getGlobal();

    if( global.getOctreeFile() == "" )
    {
        std::cerr << "Please provide octree " << std::endl;
    }
    
    
    DataHandler::Octree octree( global.getOctreeFile() );

    class defer
    {
    public:
        defer( DataHandler::Octree& o ) : octree(o) {}
        ~defer() { octree.close(); }

    private:
        DataHandler::Octree& octree;
    };

    if( !octree.open() )
        return;

    defer deferred( octree );

    if( global.getnLevels() != octree.getnLevels() )
    {
        std::cout << "nLevels not equal" << std::endl;
        return;
    }

    // Print Octree details
    std::cout << "nLevels: " << octree.getnLevels() << std::endl;
    std::cout << "Level: " << octree.getLevel() << std::endl;
    std::cout << "Max Height: " << octree.getMaxHeight() << std::endl;
    std::cout << "Isosurfaces:" << std::endl;
    for( auto i = octree.getIsosurfaces().begin();
              i != octree.getIsosurfaces().end();
              ++i )
    {
        std::cout << "\t" << *i << std::endl;
    }

    const std::vector< index_node_t * >& data = octree.getData();
    const std::vector< uint32_t >& sizes = octree.getSizes();
    uint32_t totalCubes = 0;
    uint32_t totalSize = 0;
    for( uint32_t l = 0; l <= octree.getnLevels(); l++ )
    {
        std::cout << "Level " << l << " size " << sizes[l] << ", ";
        uint32_t nC = 0;
        for( uint32_t i = 0; i < sizes[l] / 2; i+= 2 )
        {
            nC += data[ l ][ i + 1 ] - data[ l ][ i ] + 1;
        }
        std::cout << "number of cubes " << nC << std::endl;

        totalCubes += nC;
        totalSize += sizes[l];
    }

    const size_t sizeIso = totalCubes * sizeof( index_node_t ) / 1024.0 / 1024.0;
    const size_t sizeOctree = totalSize * sizeof( index_node_t ) / 1024.0 / 1024.0;
    std::cout << std::endl;
    std::cout << "Size of octree "
              << sizeOctree
              << " MB"
              << ", total cubes "
              << sizeIso
              << " MB" << std::endl;
    std::cout << "Compresion " << ( sizeOctree * 100 ) / sizeIso << "%"
              << std::endl;
}

}
}

int main( int argc, char ** argv )
{
    if( !iv::IV::init( argc, argv ) )
        return 0;

    iv::util::octreeStats();

    iv::IV::exit();
}
