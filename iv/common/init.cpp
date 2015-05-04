/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/init.h>

#include <iv/dataHandler/fileReader/factoryFileHandler.h>

#include <boost/program_options.hpp>

namespace
{

void printHelp()
{
    std::cout << "Data options:" << std::endl;
    std::cout << "--file_type:" << std::endl;
    std::cout << "    hdf5 " << IV_FILE_TYPE_HDF5 << std::endl;
    std::cout << "    ivD  " << IV_FILE_TYPE_IVD << std::endl;
    std::cout << "    test " << IV_FILE_TYPE_TEST << std::endl;
    std::cout << "--data_file:" << std::endl;
    std::cout << "    hdf5 <file-path> <dataset-name> [ x_grid ] [ y_grid ] [ z_grid ]" << std::endl;
    std::cout << "    ivD  <file-path>" << std::endl;
    std::cout << "    test <dimension>" << std::endl;
}

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

    boost::program_options::options_description cmdOptions;

    // General Options
    boost::program_options::options_description generalOptions("General options");
    generalOptions.add_options()
    ("help", "produce help message");

    // Behavior Options
    boost::program_options::options_description behaviorOptions("Behavior options");
    behaviorOptions.add_options()
    ("disable_cuda", "Disable cuda, by default enabled if available")
    ("use_hyperthreading", "Set use hyperthreading, by default disabled")
    ("max_num_threads", boost::program_options::value< uint32_t >(),
        "Select maximum number of threads")
    ;

    // Data Options
    boost::program_options::options_description dataOptions("Data options");
    dataOptions.add_options()
    ("file_type", boost::program_options::value< DataHandler::file_type_t >(),
        "Select file type, required")
    ("data_file", boost::program_options::value< DataHandler::file_args_t >()->multitoken(),
        "File arguments, required")
    ("cube_level", boost::program_options::value< level_t >(),
        "Select cube level")
    ("brick_level", boost::program_options::value< level_t >(),
        "Select brick level")
    ("cube_inc", boost::program_options::value< uint32_t >(),
        "Select cube inc")
    ("brick_inc", boost::program_options::value< uint32_t >(),
        "Select brick inc")
    ("cube_cache_size", boost::program_options::value< uint32_t >(),
        "Select cube cache size in MB")
    ("brick_cache_size", boost::program_options::value< uint32_t >(),
        "Select brick cache size in MB")
    ("octree_file", boost::program_options::value< std::string >(),
        "Octree file")
    ("octree_level", boost::program_options::value< level_t >(),
        "Select octree level, by default equal to nLevels")
    ("isos", boost::program_options::value< std::vector< float > >()->multitoken(),
        "Set of Isosurfaces")
    ;

    // Other  Options
    boost::program_options::options_description otherOptions("Other options");
    otherOptions.add_options()
    ("output_file", boost::program_options::value< std::string >(),
        "Output file")
    ;

    cmdOptions.add(generalOptions).add(behaviorOptions).add(dataOptions).add(otherOptions);

    boost::program_options::variables_map vm;
    try
    {
        boost::program_options::store(
                boost::program_options::parse_command_line( ac, av, cmdOptions ), vm );
        boost::program_options::notify( vm );
    }
    catch(...)
    {
        std::cout << cmdOptions << std::endl;
        printHelp();
        return false;
    }

    // General Options
    if( vm.count( "help" ) )
    {
        std::cout << cmdOptions << std::endl;
        printHelp();
        return false;
    }

    // Behavior Options
#ifdef IV_USE_CUDA
    if( !vm.count( "disable_cuda" ) )
        global.useCuda();
#endif
    if( vm.count( "use_hyperthreading" ) )
        global.setHyperThreading();

    if( vm.count( "max_num_threads" ) )
    {
        global.setMaxNumThreads( vm["max_num_threads"].as< uint32_t >() );
    }

    // Data parameters required
    if( !vm.count( "file_type" ) ||
        !vm.count( "data_file" ) )
    {
        std::cout << cmdOptions << std::endl;
        printHelp();
        return false;
    }
    else
    {
        const DataHandler::file_type_t fileType = vm["file_type"].as<DataHandler::file_type_t>();
        const DataHandler::file_args_t dataParam = vm["data_file"].as< DataHandler::file_args_t >();
        switch( fileType )
        {
            case IV_FILE_TYPE_HDF5:
                if( dataParam.size() < 2 )
                {
                    std::cout << cmdOptions << std::endl;
                    printHelp();
                    return false;
                }
                break;
            case IV_FILE_TYPE_IVD:
                if( dataParam.size() < 1 )
                {
                    std::cout << cmdOptions << std::endl;
                    printHelp();
                    return false;
                }
                break;
            case IV_FILE_TYPE_TEST:
                if( dataParam.size() < 1 )
                {
                    std::cout << cmdOptions << std::endl;
                    printHelp();
                    return false;
                }
                break;
            default:
                std::cout << cmdOptions << std::endl;
                printHelp();
                return false;
        }

        global.setFileType( fileType );
        global.setFileArgs( dataParam );
    }

    DataHandler::FileHandlerPtr file =
        DataHandler::FactoryFileHandler::CreateFileHandler( global.getFileType(),
                                                            global.getFileArgs() );
    if( !file )
        return false;

    const level_t nLevels = file->getnLevels();
    global.setnLevels( nLevels );

    // Data parameters optional
    if( vm.count( "cube_inc" ) )
         global.setCubeInc( vm["cube_inc"].as< uint32_t >() );

    if( vm.count( "brick_inc" ) )
         global.setBrickInc( vm["brick_inc"].as< uint32_t >() );

    if( vm.count( "cube_cache_size" ) )
        global.setCacheSizeCPU( vm["cube_cache_size"].as< uint32_t >() );

    if( vm.count( "brick_cache_size" ) )
        global.setCacheSizeGPU( vm["brick_cache_size"].as< uint32_t >() );

    if( vm.count( "cube_level" ) )
         global.setCubeLevel( vm["cube_level"].as< level_t >() );
    else // By default 64
         global.setCubeLevel( nLevels - 6 );

#ifdef IV_USE_CUDA
    if( vm.count( "brick_level" ) )
         global.setBrickLevel( vm["brick_level"].as< level_t >() );
    else // By default 32
         global.setBrickLevel( nLevels - 5 );

    if( global.getBrickLevel() < global.getCubeLevel() )
    {
        std::cout << "Brick level must be >= Cube level" << std::endl;
        std::cout << cmdOptions << std::endl;
        printHelp();
    }
#endif
    if( vm.count( "octree_file" ) )
        global.setOctreeFile( vm["octree_file"].as< std::string >() );


    if( vm.count( "octree_level" ) )
         global.setOctreeLevel( vm["octree_level"].as< level_t >() );
    else // By default nLevels
         global.setOctreeLevel( nLevels );

    if( vm.count( "isos" ) )
    {
        std::vector< float > i = vm["isos"].as< std::vector< float > >();
        std::set< float > isos( i.begin(), i.end() );
        global.setIsosurfaces( isos );
    }

    if( vm.count("output_file" ) )
        global.setOutputFile( vm["output_file"].as< std::string >() );

    return true;
}

void IV::exit()
{
    if( !_init )
        return;
}

}

