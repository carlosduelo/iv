/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <dataHandler/factoryFileHandler.h>
#include <dataHandler/hdf5FileHandler.h>

namespace iv
{

FileHandlerPtr  FactoryFileHandler::CreateFileHandler(
                    const file_type_t  type,
                    const file_args_t& file_params )
{
    switch( type )
    {
        case IV_FILE_TYPE_HDF5 :
        {
            FileHandlerPtr f( new HDF5FileHandler() );
            if( f->init( file_params ) )
                return f;

            std::cerr << "Error opening file" << std::endl;
            return 0;
        }
        default:
            std::cerr << "Unknown file type" << std::endl;
            return 0;
    }
}

}
