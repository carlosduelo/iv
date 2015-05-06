/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/fileReader/factoryFileHandler.h>
#include <iv/dataHandler/fileReader/hdf5FileHandler.h>
#include <iv/dataHandler/fileReader/ivdFileHandler.h>
#include <iv/dataHandler/fileReader/testFileHandler.h>

#include <unordered_map>

namespace iv
{

namespace DataHandler
{

FileHandlerPtr  FactoryFileHandler::CreateFileHandler(
                    const file_type_t  type,
                    const file_args_t& file_params )
{
    static std::unordered_map< std::string, FileHandlerPtr > openFiles;

    auto file = openFiles.find( file_params[0] );

    if( file != openFiles.end() )
        return file->second;

    switch( type )
    {
        case IV_FILE_TYPE_HDF5 :
        {
            FileHandlerPtr f( new HDF5FileHandler(),
                              [](FileHandler* ptr){ ptr->close();} );
            if( f->init( file_params ) )
            {
                openFiles[ file_params[0] ] = f;
                return f;
            }

            std::cerr << "Error opening file" << std::endl;
            return 0;
        }
        case IV_FILE_TYPE_IVD :
        {
            FileHandlerPtr f( new IVDFileHandler(),
                              [](FileHandler* ptr){ ptr->close();} );
            if( f->init( file_params ) )
            {
                openFiles[ file_params[0] ] = f;
                return f;
            }

            std::cerr << "Error opening file" << std::endl;
            return 0;
        }
        case IV_FILE_TYPE_TEST :
        {
            FileHandlerPtr f( new TestFileHandler(),
                              [](FileHandler* ptr){ ptr->close();} );
            if( f->init( file_params ) )
            {
                openFiles[ file_params[0] ] = f;
                return f;
            }

            std::cerr << "Error opening file" << std::endl;
            return 0;
        }
        default:
            std::cerr << "Unknown file type" << std::endl;
            return 0;
    }
}

}

}
