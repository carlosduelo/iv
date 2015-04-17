/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>

#include <hdf5.h>

namespace iv
{

namespace util
{

void cutHDF5( const int argc, char ** const argv )
{
    if( argc != 10 )
    {
        std::cerr << "Usage <in-file> <out-file> <dataset-name> "
            << " offsetX offsetY offsetZ "
            << " dimX dimY dimZ"
            << std::endl;

        return;
    }

    DataHandler::file_type_t file_type = IV_FILE_TYPE_HDF5;;
    DataHandler::file_args_t file_args;
    file_args.push_back( argv[1] );
    file_args.push_back( argv[3] );
    DataHandler::FileHandlerPtr fileH5 =
            DataHandler::FactoryFileHandler::CreateFileHandler( file_type,
                                                                file_args );
    if( !fileH5 )
    {
        std::cerr << "Error opening file " << argv[1] << std::endl;
        return;
    }

    const uint32_t offsetX = atoi( argv[4] );
    const uint32_t offsetY = atoi( argv[5] );
    const uint32_t offsetZ = atoi( argv[6] );
    const uint32_t dimX = atoi( argv[7] );
    const uint32_t dimY = atoi( argv[8] );
    const uint32_t dimZ = atoi( argv[9] );


    const vec3int32_t start( offsetX, offsetY, offsetZ );
    const vec3int32_t end = start + vec3int32_t( dimX, dimY, dimZ );
    std::unique_ptr< float[] > data( new float[ dimX * dimY * dimZ ] );
    fileH5->read( data.get(), start, end );

    hid_t       file, dataset;         /* file and dataset handles */
    hid_t       datatype, dataspace;   /* handles */
    hsize_t     dimsf[3];              /* dataset dimensions */
    herr_t      status;

    if( ( file = H5Fcreate( argv[2], H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT ) ) < 0 )
    {
        std::cerr << " unable to create the HDF5 file" << std::endl;
        return;
    }

    dimsf[0] = (hsize_t)dimX;
    dimsf[1] = (hsize_t)dimY;
    dimsf[2] = (hsize_t)dimZ;

    if( ( dataspace = H5Screate_simple( 3, dimsf, NULL ) ) < 0 )
    {
        std::cerr << " unable to create the data space" << std::endl;
        return; 
    }

    if( ( datatype = H5Tcopy( H5T_IEEE_F32LE ) ) < 0 )
    {
        std::cerr << " unable to create the data type" << std::endl;
        return;
    }

    if( ( status = H5Tset_order( datatype, H5T_ORDER_LE ) ) < 0 )
    {
        std::cerr << " unable to set order" << std::endl;
        return;
    }

    if( ( dataset = H5Dcreate2(file, argv[3], datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT ) ) < 0 )
    {
        std::cerr << " unable to create the data set" << std::endl;
        return;
    }

    if( ( status = H5Dwrite( dataset, H5T_IEEE_F32LE, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.get() ) ) < 0 )
    {
        std::cerr << " unable to write data set" << std::endl;
        return;
    }

    if( H5Sclose( dataspace ) < 0 )
    {
        std::cerr << " unable to close the dataspace" << std::endl;
        return;
    }

    if( H5Tclose( datatype ) < 0 )
    {
        std::cerr << " unable to close the datatype" << std::endl;
        return;
    }

    if( H5Dclose( dataset ) < 0 )
    {
        std::cerr << " unable to close the data set" << std::endl;
        return;
    }

    if( H5Fclose( file ) < 0 )
    {
        std::cerr << " unable to close the file" << std::endl;
        return;
    }
}
}
}

int main( int argc, char ** argv )
{
    if( !iv::init( argc, argv ) )
        return 0;

    iv::util::cutHDF5( argc, argv ); 

    iv::exit();

    return 0;
}
