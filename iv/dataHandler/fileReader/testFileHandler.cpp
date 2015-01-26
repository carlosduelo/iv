/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/fileReader/testFileHandler.h>

#include <stdlib.h>
#include <string.h>

namespace iv
{

namespace DataHandler
{

bool TestFileHandler::init(
                const file_args_t& file_params )
{
    const int sizeParam = file_params.size();

    if( sizeParam != 1 )
        return false;

    unsigned dim = 0;
    try
    {
        int dims = atoi( file_params[0].c_str() );
        if( dims <= 0 )
            return false;
        dim = dims;
    }
    catch(...)
    {
        return false;
    }

    // Set RealDimension
    _realDimension.set( dim, dim, dim );

    _grid.reset( new float[ dim ] );
    for( unsigned i = 0; i < dim; i++ )
        _grid[i] = (float) i;

    _data.reset( new float[ dim * dim * dim ] );
    memset( _data.get(), 0, dim * dim * dim * sizeof( float ) );

    float iso = 0.1;
    float cx = dim / 2;
    float cy = cx;
    float cz = cx;
    for( float r = 4.0; r < dim / 2.0; r+= 4.0 )
    {
        for(unsigned i = 0; i < dim; i++ )
            for(unsigned j = 0; j < dim; j++ )
                for(unsigned k = 0; k < dim; k++ )
                {
                    float c = ( ( i - cx ) * ( i - cx ) ) +
                              ( ( j - cy ) * ( j - cy ) ) +
                              ( ( k - cz ) * ( k - cz ) ) -
                              ( r * r );
                    if( c == 0 )
                        _data[ i * dim * dim + j * dim + k ] = iso;
                }
        iso += 0.3;
    }

    return true;
}

void TestFileHandler::_close( )
{
}

const float * TestFileHandler::getxGrid( ) const
{
    assert( _grid );
    return _grid.get();
}

const float * TestFileHandler::getyGrid( ) const
{
    assert( _grid );
    return _grid.get();
}

const float * TestFileHandler::getzGrid( ) const
{
    assert( _grid );
    return _grid.get();
}

void TestFileHandler::read( float * const data,
           const vec3int32_t& s,
           const vec3int32_t& e
         ) const
{
	hsize_t dim[3] = { ( hsize_t ) abs( e.x() - s.x() ),
                       ( hsize_t ) abs( e.y() - s.y() ),
                       ( hsize_t ) abs( e.z() - s.z() )
                     };

	// Set zeros's
	memset( ( void* ) data, 0, dim[0] * dim[1] * dim[2] * sizeof( float ) );

	if( s.x() >= ( int ) this->_realDimension[0] ||
        s.y() >= ( int ) this->_realDimension[1] ||
        s.z() >= ( int ) this->_realDimension[2] ||
        e.x() < 0 ||
        e.y() < 0 ||
        e.z() < 0 )
	{
        return;
    }

	unsigned offset[3] 	= {s.x() < 0 ? 0 : ( unsigned ) s.x(),
                           s.y() < 0 ? 0 : ( unsigned ) s.y(),
                           s.z() < 0 ? 0 : ( unsigned ) s.z()
                           };

	unsigned dimR[3]	= {e.x() > _realDimension[0] ?
                                        _realDimension[0] - offset[0] :
                                        e.x() - offset[0],
				           e.y() > _realDimension[1] ?
                                        _realDimension[1] - offset[1] :
                                        e.y() - offset[1],
				           e.z() > _realDimension[2] ?
                                        _realDimension[2] - offset[2] :
                                        e.z() - offset[2]
                           };

    unsigned index = 0;
    for( unsigned i = offset[0]; i < dimR[0]; i++ )
        for( unsigned j = offset[0]; j < dimR[0]; j++ )
            for( unsigned k = offset[0]; k < dimR[0]; k++ )
            {
                data[ index ] = _data[ i * _realDimension[0] * _realDimension[0] +
                                       j * _realDimension[1] +
                                       k 
                                     ];
                index++;
            }
}

}

}
