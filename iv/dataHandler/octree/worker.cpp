/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/octreeConstructor.h>

namespace iv
{

namespace DataHandler
{

void Worker::addCube( const index_node_t cube )
{
    _cubes.push_back( cube );
}

bool Worker::start()
{
    _thread = std::thread( &Worker::_run, this );
    return true;
}

void Worker::_run()
{
    for( auto i = _cubes.begin(); i != _cubes.end(); i++ )
    {
        if( !_computeCube( *i ) )
            std::cerr << "Error computing cube " << *i << std::endl;
    }
}

void Worker::wait()
{
    if( _thread.joinable() )
        _thread.join();
}

}

}
