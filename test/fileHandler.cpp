/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>
#include <iv/dataHandler/factoryFileHandler.h>

#include <boost/program_options.hpp>

#include <hdf5.h>
#include <hdf5_hl.h>

#define dimx 100
#define dimy 100
#define dimz 100

#define posToIndex(i,j,k,d) ((k)+(j)*(d)+(i)*(d)*(d))


/*
 * Aux functions
 */

void write_cube_hdf5(
        const float *data,
        hsize_t nx,
        hsize_t ny,
        hsize_t nz,
        const char *fname )
{
    /* now the various steps involved in preparing an hdf5 file */
    hid_t file_id;
    /* at this time our basic file just has the values strung out,
       so the hdf5 rank is 1 */
    hsize_t dims[3] = {nx, ny, nz};
    herr_t status;

    /* create a HDF5 file */
    file_id = H5Fcreate(fname, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    /* create and write a float type dataset named "/datacube" */
    status = H5LTmake_dataset(file_id, "/datacube", 3, dims,
            H5T_IEEE_F32LE, data);
    assert(status != -1);
    /* add some hdf5 attributes with the metadata we need */
    H5LTset_attribute_int(file_id, "/datacube", "nx", (const int*)&nx, 1);
    H5LTset_attribute_int(file_id, "/datacube", "nx", (const int*)&nx, 1);
    H5LTset_attribute_int(file_id, "/datacube", "ny", (const int*)&ny, 1);
    H5LTset_attribute_string(file_id, "/datacube", "x_units", "m");
    H5LTset_attribute_string(file_id, "/datacube", "y_units", "kg");
    H5LTset_attribute_string(file_id, "/datacube", "z_units", "degK");

    status = H5Fclose (file_id); assert(status != -1);
    assert(status != -1);
}

const float *fill_cube( int nx, int ny, int nz )
{
    int i, j, k;
    float x, y, z;
    float *data;
    data = new float[ nx*ny*nz ];
    for (i = 0; i < nx; ++i) {
        for (j = 0; j < ny; ++j) {
            for (k = 0; k < nz; ++k) {
                x = (i - nx/2)/100.0;
                y = (j - ny/2)/100.0;
                z = (k - nz/2)/100.0;
                float val = (exp((-(z)*(z)/1.0
                                -(y)*(y)/1.0)
                            * (i+1)*(i+1)/10.0)
                        * (x+1)*(x+1));
                data[i*ny*nz + j*nz + k] = val;
            }
        }
    }
    return data;
}

bool checkDimension( iv::DataHandler::FileHandlerPtr& file )
{
    const iv::vec3int32_t& dim = file->getRealDimension();
    if( dim.x() != dimx || dim.y() != dimy || dim.z() != dimz )
    {
        std::cerr << "Error not equal dimensions " << dim << " ( "
                  << dimx << ", " << dimy << ", " << dimz << " )"
                  << std::endl;
        return false;
    }

    return true;
}

bool readAndCheck( const iv::DataHandler::FileHandlerPtr& file,
                   const float * cube,
                   const iv::vec3int32_t& start,
                   const iv::vec3int32_t& end
                 )
{
    const unsigned size = abs( end.x() - start.x() ) *
                          abs( end.y() - start.y() ) *
                          abs( end.z() - start.z() );

    float * const read = new float[ size ];

    // Reading
    file->read( read, start, end );

    // Cecking
    const int lx = start.x() < 0 ? 0 : start.x();
    const int Lx = end.x() > dimx ? dimx : end.x();
    const int ly = start.y() < 0 ? 0 : start.y();
    const int Ly = end.y() > dimy ? dimy : end.y();
    const int lz = start.z() < 0 ? 0 : start.z();
    const int Lz = end.z() > dimz ? dimz : end.z();
    int x = start.x() < 0 ? abs( start.x() ) : 0;
    int y = start.y() < 0 ? abs( start.y() ) : 0;
    int z = start.z() < 0 ? abs( start.z() ) : 0;
    const int sizeRead = abs( end.z() - start.z() );

    bool result = true;
    for( int i = lx; i < Lx && result; i++)
    {
        y = start.y() < 0 ? abs( start.y() ) : 0;
        for( int j = ly; j < Ly && result; j++)
        {
            z = start.z() < 0 ? abs( start.z() ) : 0;
            for( int k = lz; k < Lz && result; k++)
            {
                if( cube[ posToIndex( i, j, k, dimx ) ] !=
                    read[ posToIndex( x, y, z, sizeRead ) ] )
                {
                    #if 0
                    std::cout << x << " " << y << " " << z << " "
                              << posToIndex( x, y, z, sizeRead ) << std::endl;

                    std::cout << i << " " << j << " " << k << " "
                              << posToIndex( i, j, k, dimx ) << std::endl;
                    #endif
                    std::cerr << "Error reading not equal" << std::endl;
                    result = false;
                }
                z++;
            }
            y++;
        }
        x++;
    }

    delete[] read;

    return result;
}

int main( int ac, char ** av )
{
    boost::program_options::options_description desc("Allowed options:");
    desc.add_options()
    ("help", "produce help message")
    ("file-type",
     boost::program_options::value<unsigned>()->default_value(IV_FILE_TYPE_HDF5),
     "Select file type")
    ;

    boost::program_options::variables_map vm;
    try
    {
        boost::program_options::store(
                boost::program_options::parse_command_line( ac, av, desc ), vm );
        boost::program_options::notify( vm );
    }
    catch(...)
    {
        std::cout << desc << std::endl;
        std::cout << "file-type, default value hdf5" << std::endl;
        std::cout << "    hdf5 " << IV_FILE_TYPE_HDF5 << std::endl;
        return 1;
    }

    if( vm.count( "help" ) ) {
        std::cout << desc << std::endl;
        std::cout << "file-type, default value hdf5" << std::endl;
        std::cout << "    hdf5 " << IV_FILE_TYPE_HDF5 << std::endl;
        return 1;
    }

    const unsigned fileType = vm["file-type"].as<unsigned>();

    std::vector<std::string> file_params;
    const float * cube = fill_cube( dimx, dimy, dimz );

    switch( fileType )
    {
        case IV_FILE_TYPE_HDF5 :
        {
            std::cout << "Testing HDF5 file" << std::endl;
            std::string fname = "sample_cube.h5";
            write_cube_hdf5( cube, dimx, dimy, dimz, fname.c_str() );
            file_params.push_back( fname );
            file_params.push_back( "datacube" );
            break;
        }
        default:
            std::cerr << "Unknown file type" << std::endl;
            delete[] cube;
            return 0;
    }

    iv::DataHandler::FileHandlerPtr file =
            iv::DataHandler::FactoryFileHandler::CreateFileHandler(
                                                        fileType, file_params );

    if( !file )
    {
        delete[] cube;
        std::cerr << "Error testing " << av[0] <<std::endl;
        return 1;
    }

    // Check dimension
    const bool result =
        checkDimension( file ) &&
        // Read Planes
        readAndCheck( file, cube,
                      iv::vec3int32_t(0,0,0),
                      iv::vec3int32_t(1, dimy, dimz ) ) &&
        readAndCheck( file, cube,
                      iv::vec3int32_t(-1,0,0),
                      iv::vec3int32_t(0, dimy, dimz ) ) &&
        readAndCheck( file, cube,
                      iv::vec3int32_t(50,0,0),
                      iv::vec3int32_t(51, dimy, dimz ) ) &&
        readAndCheck( file, cube,
                      iv::vec3int32_t(99,0,0),
                      iv::vec3int32_t(100, dimy, dimz ) ) &&
        // Read Cubes
        readAndCheck( file, cube,
                      iv::vec3int32_t(0,0,0),
                      iv::vec3int32_t(50, 50, 50 ) ) &&
        readAndCheck( file, cube,
                      iv::vec3int32_t(-1,-1,-1),
                      iv::vec3int32_t(5, 5, 5 ) ) &&
        readAndCheck( file, cube,
                      iv::vec3int32_t(49,49,49),
                      iv::vec3int32_t(dimx, dimy, dimz ) ) &&
        readAndCheck( file, cube,
                      iv::vec3int32_t(0,0,0),
                      iv::vec3int32_t(dimx, dimy, dimz ) );

    delete[] cube;

    if( result )
    {
        std::cout << "Test OK" <<std::endl;
        return 0;
    }
    else
    {
        std::cout << "Test Fail" <<std::endl;
        return 0;
        return 1;
    }
}

