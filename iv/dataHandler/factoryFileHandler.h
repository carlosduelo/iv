/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_FACTORYFILEHANDLER
#define _IV_FACTORYFILEHANDLER

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileHandler.h>

namespace iv
{

namespace DataHandler
{

/*
 * File Type = HDF5
 * file_params = file_name data_set_name [ xgrid  ygrid zgrid ]
 *
 */

class FactoryFileHandler
{
public:
static FileHandlerPtr CreateFileHandler(
                                        const file_type_t  type,
                                        const file_args_t& file_params );
};

}

}

#endif // _IV_FACTORYFILEHANDLER
