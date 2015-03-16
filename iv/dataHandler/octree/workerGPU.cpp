/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/octree/workerCPU.h>

#include <iv/dataHandler/cache/cache.h>
#include <iv/dataHandler/cache/objectHandler.h>

#include <iv/dataHandler/octree/octreeConstructorAttr.h>
#include <iv/dataHandler/octree/dataWarehouse.h>

#include <iv/common/mortonCodeUtil_CPU.h>

#include <chrono>

#define posToIndex(i,j,k,d) ((k)+(j)*(d)+(i)*(d)*(d))

namespace iv
{

namespace DataHandler
{

bool checkIsosurface( const int x, const int y, const int z,
                      const int dim, const float * cube, const float iso )
{
    const bool sign = ( cube[ posToIndex( x, y, z, dim ) ] - iso ) < 0;

    if( ( ( cube[ posToIndex( x, y, z+1, dim ) ] - iso ) < 0 ) != sign )
       return true;
    if( ( ( cube[ posToIndex( x, y+1, z, dim ) ] - iso ) < 0 ) != sign )
       return true;
    if( ( ( cube[ posToIndex( x, y+1, z+1, dim ) ] - iso ) < 0 ) != sign )
       return true;
    if( ( ( cube[ posToIndex( x+1, y, z, dim ) ] - iso ) < 0 ) != sign )
       return true;
    if( ( ( cube[ posToIndex( x+1, y, z+1, dim ) ] - iso ) < 0 ) != sign )
       return true;
    if( ( ( cube[ posToIndex( x+1, y+1, z, dim ) ] - iso ) < 0 ) != sign )
       return true;
    if( ( ( cube[ posToIndex( x+1, y+1, z+1, dim ) ] - iso ) < 0 ) != sign )
        return true;

    return false;
}

bool WorkerCPU::_computeCube( const index_node_t cube )
{
    const float bytesRead = powf(
                            exp2( _attr->getnLevels() -
                                  _attr->getReadLevel() ) + 2 * _attr->getCubeInc(), 3 ) *
                          sizeof(float);
    auto startR = std::chrono::high_resolution_clock::now();
        ObjectHandlerPtr o = _cache->get( cube );
        const float* data = o->lock();
    auto endR = std::chrono::high_resolution_clock::now();
    assert( data );

    auto startC = std::chrono::high_resolution_clock::now();
        _computeCubeData( cube, data );
    auto endC = std::chrono::high_resolution_clock::now();

    _stats.incReadBytes( bytesRead );
    _stats.incCube();
    _stats.incReadingTime(std::chrono::duration_cast<
                                            std::chrono::milliseconds>(
                                                endR - startR ) );
    _stats.incComputingTime(std::chrono::duration_cast<
                                            std::chrono::milliseconds>(
                                                endC - startC ) );
    return true;
}

void WorkerCPU::_computeCubeData( const index_node_t id,
                                                const float * cube )
{
    index_node_t idS = id <<
                       3 * ( _attr->getLevel() - _attr->getReadLevel() );
    index_node_t idE = (id + 1) <<
                       3 * ( _attr->getLevel() - _attr->getReadLevel() );

    const uint32_t dimCube = exp2f(
                               _attr->getnLevels() - _attr->getLevel() );
    const uint32_t dimCubeData = exp2f(
                                   _attr->getnLevels() -
                                   _attr->getReadLevel() ) + 2 * _attr->getCubeInc();

    for( index_node_t i = idS; i < idE; i++ )
    {
        if( _computeCube( i, cube, dimCube, dimCubeData ) )
            return;
            _data->pushCube( i );
    }
}

bool WorkerCPU::_computeCube( const index_node_t id,
                                            const float* cube,
                                            const uint32_t dimCube,
                                            const uint32_t dimCubeData )
{
    vec3int32_t coordStart = getMinBoxIndex2( id,
                                              _attr->getLevel(),
                                              _attr->getnLevels() );
    vec3int32_t coordEnd = coordStart + vec3int32_t( dimCube, dimCube, dimCube );
    const uint32_t cubeInc = _attr->getCubeInc();

    for( int32_t i = coordStart.x(); i < coordEnd.x(); i++ )
        for( int32_t j = coordStart.y(); j < coordEnd.y(); j++ )
            for( int32_t k = coordStart.z(); k < coordEnd.z(); k++ )
            {
                for( auto iso = _attr->getIsosurfaces().begin();
                          iso != _attr->getIsosurfaces().end();
                          iso++ )
                {
                    if( checkIsosurface( i+cubeInc, j+cubeInc, k+cubeInc,
                                                    dimCubeData, cube, *iso ) )
                        return true;
                }
            }

    return false;
}

}

}

