/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/common/types.h>
#include <iv/common/mortonCodeUtil_CPU.h>

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileReader/factoryFileHandler.h>

int main( int ac, char ** av )
{
    if( ac != 6 )
    {
        std::cout << "Usage <hdf5-file> <datase> <ivd-file> <level> <cube-inc>" << std::endl;
        return 0;
    }

    const unsigned fileTypeH5 = IV_FILE_TYPE_HDF5;
    std::vector<std::string> file_paramsH5;
    file_paramsH5.push_back( av[1] );
    file_paramsH5.push_back( av[2] );
    const iv::level_t level = atoi( av[4] );
    const int32_t cubeInc = atoi( av[5] );

    const unsigned fileTypeIVD = IV_FILE_TYPE_IVD;
    std::vector<std::string> file_paramsIVD;
    file_paramsIVD.push_back( av[3] );

    iv::DataHandler::FileHandlerPtr fileH5 =
            iv::DataHandler::FactoryFileHandler::CreateFileHandler(
                                                        fileTypeH5, file_paramsH5 );

    iv::DataHandler::FileHandlerPtr fileIV =
            iv::DataHandler::FactoryFileHandler::CreateFileHandler(
                                                        fileTypeIVD, file_paramsIVD );

    // Compare nLevels 
    if( fileH5->getnLevels() != fileIV->getnLevels() )
    {
        std::cout << "nLevels differ" << std::endl;
        return 0;
    }

    // Compare Dimensions
    if( fileH5->getRealDimension() != fileIV->getRealDimension() )
    {
        std::cout << "Dimensions differ" << std::endl;
        return 0;
    }

    // Compare Grids
    for( int32_t i = 0; i < fileIV->getRealDimension().x(); i++ )
        if( fileIV->getxGrid()[i] != fileH5->getxGrid()[i] )
        {
            std::cout << "Grids are different" << std::endl;
            return 0;
        }
    for( int32_t i = 0; i < fileIV->getRealDimension().y(); i++ )
        if( fileIV->getyGrid()[i] != fileH5->getyGrid()[i] )
        {
            std::cout << "Grids are different" << std::endl;
            return 0;
        }
    for( int32_t i = 0; i < fileIV->getRealDimension().z(); i++ )
        if( fileIV->getzGrid()[i] != fileH5->getzGrid()[i] )
        {
            std::cout << "Grids are different" << std::endl;
            return 0;
        }
    
    // Compare data
    const iv::level_t nLevels = fileH5->getnLevels();
    const uint32_t dim = exp2f( nLevels - level ) + 2 * cubeInc;
    const uint32_t cubeSize = powf( dim, 3 );
    const iv::index_node_t idStart  = 1 << 3 * level;
    const iv::index_node_t idFinish = ( 2 << 3 * level ) - 1;

    std::cout << "Comparing cubes: " << idStart << " " << idFinish << std::endl;
    std::unique_ptr< float[] > dataH5( new float[ cubeSize ] );
    std::unique_ptr< float[] > dataIV( new float[ cubeSize ] );

    for( iv::index_node_t id = idStart; id <= idFinish; id++ )
    {
        fileH5->readCube( dataH5.get(), id, level, nLevels, cubeInc );
        fileIV->readCube( dataIV.get(), id, level, nLevels, cubeInc );
        for( uint32_t i = 0; i < cubeSize; i++ )
        {
            if( dataH5[i] != dataIV[i] )
            {
                std::cerr << "Error results differ in cube " << id << " "
                          << iv::getMinBoxIndex2( id, level, nLevels ) << " "
                          << i << " "
                          << dataH5[i] << " "
                          << dataIV[i]
                          << std::endl;
                          #if 0
                for( uint32_t j = 0; j < cubeSize; j++ )
                          std::cout << dataH5[j] << " "
                          << dataIV[j] << std::endl;
                          #endif
                return 0;
            }
        }
    }

}
