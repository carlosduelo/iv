/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>

#include <boost/program_options.hpp>

namespace
{
    bool _init = false;
}

namespace iv
{

const Global& IV::getGlobal()
{
    return Global::getInstance();
}

bool IV::init( const int ac, char ** av )
{
    if( _init )
        return true;
    _init = true;

    Global& global = Global::getInstance();

    // Parsing arguments
    boost::program_options::options_description desc("Allowed options:");
    desc.add_options()
    ("help", "produce help message")
    ("file-type",
     boost::program_options::value<DataHandler::file_type_t>()->default_value(IV_FILE_TYPE_HDF5),
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
        return false;
    }

    if( vm.count( "help" ) ) {
        std::cout << desc << std::endl;
        std::cout << "file-type, default value hdf5" << std::endl;
        std::cout << "    hdf5 " << IV_FILE_TYPE_HDF5 << std::endl;
        std::cout << "    ivD  " << IV_FILE_TYPE_IVD << std::endl;
        return false;
    }

    const DataHandler::file_type_t fileType = vm["file-type"].as<DataHandler::file_type_t>();
    global.setFileType( fileType );

    return true;
}

void IV::exit()
{
    if( !_init )
        return;
}

}

