/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/hdf5FileHandler.h>

namespace iv
{

namespace DataHandler
{

bool HDF5FileHandler::init(
                const file_args_t& file_params )
{
    const int sizeParam = file_params.size();

    if( sizeParam != 2 && sizeParam != 5 )
        return false;

    if( ( _file_id = H5Fopen( file_params[0].c_str(),
                              H5F_ACC_RDWR,
                              H5P_DEFAULT ) ) < 0 )
    {
        std::cerr << "FileHandler: Error opening "
                  << file_params[0] << std::endl;
        return false;
    }

    if( ( _dataset_id = H5Dopen1( _file_id, file_params[1].c_str() ) ) < 0 )
    {
        if( H5Fclose( _file_id ) < 0 )
            std::cerr << "hdf5: unable to close the file" << std::endl;
        std::cerr << "hdf5: unable to open the requested data set "
                  << file_params[1] << std::endl;
        return false;
    }

    if( ( _datatype = H5Dget_type( _dataset_id ) ) < 0 )
    {
        std::cerr << "hdf5: unable to data set type" << std::endl;
        return false;
    }

    if( ( _spaceid = H5Dget_space( _dataset_id ) ) < 0 )
    {
        std::cerr << "hdf5: unable to open the requested data space"
                  << std::endl;
        return false;
    }

    if( ( _ndim = H5Sget_simple_extent_dims ( _spaceid, _dims, NULL ) ) < 0 )
    {
        std::cerr << "hdf5: handling file" << std::endl;
        return false;
    }

    // Set RealDimension
    _realDimension.set( _dims[0], _dims[1], _dims[2] );

    _xGrid.reset( new float[ _dims[0] ] );
    _yGrid.reset( new float[ _dims[1] ] );
    _zGrid.reset( new float[ _dims[2] ] );

    const bool readGrid =
    _readGrid( sizeParam == 5 ? file_params[2] : "", _xGrid.get(), _dims[0] ) &&
    _readGrid( sizeParam == 5 ? file_params[3] : "", _yGrid.get(), _dims[1] ) &&
    _readGrid( sizeParam == 5 ? file_params[4] : "", _zGrid.get(), _dims[2] );

    _isOpen = true;

    return readGrid;
}

const float * HDF5FileHandler::getxGrid( ) const
{
    assert( _xGrid );
    return _xGrid.get();
}

const float * HDF5FileHandler::getyGrid( ) const
{
    assert( _yGrid );
    return _yGrid.get();
}

const float * HDF5FileHandler::getzGrid( ) const
{
    assert( _zGrid );
    return _zGrid.get();
}

void HDF5FileHandler::read( float * const data,
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

	// The data required is completly outside of the dataset
	if( s.x() >= ( int ) this->_dims[0] ||
        s.y() >= ( int ) this->_dims[1] ||
        s.z() >= ( int ) this->_dims[2] ||
        e.x() < 0 ||
        e.y() < 0 ||
        e.z() < 0 )
	{
		#if 0
		std::cerr << "Warning: reading data outsite the volume" << std::endl;
		std::cerr << "Dimension volume " << this->_dims[0] << " "
                  << this->_dims[1] << " " << this->_dims[2] << std::endl;
		std::cerr << "start " << s.x() << " " << s.y() << " " << s.z()
                  << std::endl;
		std::cerr << "end " << e.x() << " " << e.y() << " " << e.z()
                  << std::endl;
		std::cerr << "Dimension data " << dim[0] << " " << dim[1] << " "
                  << dim[2] << std::endl;
		#endif
		return;
	}

	herr_t	status;
	hid_t	memspace;
	hsize_t offset_out[3] = {s.x() < 0 ? ( hsize_t ) abs( s.x() ) : 0,
                             s.y() < 0 ? ( hsize_t ) abs( s.y() ) : 0,
                             s.z() < 0 ? ( hsize_t ) abs( s.z() ) : 0
                             };
	hsize_t offset[3] 	= {s.x() < 0 ? 0 : ( hsize_t ) s.x(),
                           s.y() < 0 ? 0 : ( hsize_t ) s.y(),
                           s.z() < 0 ? 0 : ( hsize_t ) s.z()
                           };
	hsize_t dimR[3]		= {e.x() > ( int ) this->_dims[0] ?
                                this->_dims[0] - offset[0] : e.x() - offset[0],
				           e.y() > ( int ) this->_dims[1] ?
                                this->_dims[1] - offset[1] : e.y() - offset[1],
				           e.z() > ( int ) this->_dims[2] ?
                                this->_dims[2] - offset[2] : e.z() - offset[2]
                          };

	/*
	* Define hyperslab in the dataset.
	*/
	if( ( status = H5Sselect_hyperslab(_spaceid, H5S_SELECT_SET,
                                            offset, NULL, dimR, NULL ) ) < 0 )
	{
		std::cerr << "hdf5: defining hyperslab in the dataset" << std::endl;
	}

	/*
	* Define the memory dataspace.
	*/
	if( ( memspace = H5Screate_simple( 3, dim, NULL ) ) < 0 )
	//if ((memspace = H5Screate_simple(3, dimR, NULL)) < 0)
	{
		std::cerr << "hdf5: defining the memory space" << std::endl;
	}

	/*
	* Define memory hyperslab.
	*/
	if( ( status = H5Sselect_hyperslab ( memspace, H5S_SELECT_SET, offset_out,
                                            NULL, dimR, NULL ) ) < 0 )
	{
		std::cerr << "hdf5: defining the memory hyperslab" << std::endl;
	}

	/*
	* Read data from hyperslab in the file into the hyperslab in
	* memory and display.
	*/
	if( ( status = H5Dread(_dataset_id, H5T_IEEE_F32LE/*_datatype*/, memspace,
                            _spaceid, H5P_DEFAULT, ( void* ) data ) ) < 0 )
	{
		std::cerr << "hdf5: reading data from hyperslab un the file"
                  << std::endl;
	}

	if( ( status = H5Sclose( memspace ) ) < 0 )
	{
		std::cerr << "hdf5: closing dataspace" << std::endl;
	}
}

void HDF5FileHandler::_close( )
{
	if( H5Dclose( _dataset_id ) < 0 )
	{
		std::cerr << "hdf5: unable to close the data set" << std::endl;
	}

	if( H5Fclose( _file_id ) < 0 )
	{
		std::cerr << "hdf5: unable to close the file" << std::endl;
		/*
		 * XXX cduelo: No entiendo porque falla al cerrar el fichero....
		 *
		 */
	}
}

bool HDF5FileHandler::_readGrid( const std::string& name,
                                 float * const grid,
                                 const hsize_t dim )
{
	if (name == "")
	{
		for(hsize_t i=0; i<dim; i++)
			grid[i] = ( float ) i;
	}
	else
	{
		double * g = new double[dim];

		hid_t   dataset_id;
		hid_t   spaceid;
		hid_t	datatype;

		if( ( dataset_id = H5Dopen1( _file_id, name.c_str() ) ) < 0 )
		{
			std::cerr << "hdf5: unable to open the requested data set "
                      << name << std::endl;
			delete[] g;
			return false;
		}

		if( ( datatype = H5Dget_type( dataset_id ) ) < 0 )
		{
			std::cerr << "hdf5: unable to data set type" << std::endl;
			if( H5Dclose( dataset_id ) < 0 )
				std::cerr << "hdf5: unable to close the data set" << std::endl;
			delete[] g;
			return false;
		}

		if( ( spaceid = H5Dget_space( dataset_id ) ) < 0 )
		{
			std::cerr << "hdf5: unable to open the requested data space"
                      << std::endl;
			if( H5Dclose( dataset_id ) < 0 )
				std::cerr << "hdf5: unable to close the data set" << std::endl;
			delete[] g;
			return false;
		}

		int ndim = 0;
		hsize_t dims[3];
		if( ( ndim = H5Sget_simple_extent_dims( spaceid, dims, NULL ) ) < 0 )
		{
			std::cerr << "hdf5: handling file" << std::endl;
			if( H5Dclose( dataset_id ) < 0 )
				std::cerr << "hdf5: unable to close the data set" << std::endl;
			delete[] g;
			return false;
		}
		if( ndim != 1 || dims[0] != dim )
		{
			std::cerr << "hdf5: grid dimension no equal to data dimension"
            << std::endl;
			if( H5Dclose( dataset_id ) < 0 )
				std::cerr << "hdf5: unable to close the data set" << std::endl;
			delete[] g;
			return false;
		}

		if( H5Dread( dataset_id, H5T_IEEE_F64LE/*datatype*/,
                     H5S_ALL, spaceid, H5P_DEFAULT, g ) < 0 )
		{
			std::cerr << "hdf5: reading grid" << std::endl;
			if( H5Dclose( dataset_id ) < 0 )
				std::cerr << "hdf5: unable to close the data set" << std::endl;
			delete[] g;
			return false;
		}

		if( H5Dclose( dataset_id ) < 0 )
		{
			delete[] g;
			std::cerr << "hdf5: unable to close the data set" << std::endl;
			return false;
		}

		for( hsize_t i = 0; i < dim; i++)
			grid[i] = ( float ) g[i];
		delete[] g;
	}

	return true;
}

}

}
