/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <dataHandler/fileHandler.h>

namespace iv
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
