/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_DATAWAREHOUSE_H_
#define _IV_OCTREE_DATAWAREHOUSE_H_

#include <thread>
#include <fstream>
#include <unordered_map>

namespace iv
{

namespace DataHandler
{

class DataWarehouse
{
public:
    DataWarehouse( const index_node_t min, const index_node_t max )
        : _max( max )
        , _min( min )
    {
    }

    bool start();

    void wait();

    void pushCube( const index_node_t id );

private:
    std::thread             _thread;
    Queue< index_node_t >   _queue;

    const index_node_t      _max;
    const index_node_t      _min;

    std::vector< index_node_t >                         _indices;
    std::vector< uint32_t >                             _dimensions;
    std::vector< std::shared_ptr< std::ofstream > >     _tmpFiles;
    std::vector< std::string >                          _nameTmpFiles;

    std::ofstream   _endFile;
    std::string     _nameEndFile;

    void _run( );

    void _addCube( const index_node_t cube );; 

    void _sort();

    void _fileToVector( std::ifstream& file,
                        std::vector< index_node_t >& vector,
                        const uint32_t dim );

    void _sortVector( std::vector< index_node_t >& vector );

    void _vectorToFile( std::ofstream& file,
                        const std::vector< index_node_t >& vector,
                        const uint32_t dim );
};

}
}

#endif // _IV_OCTREE_DATAWAREHOUSE_H_

