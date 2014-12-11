/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_TYPES_H_
#define _IV_TYPES_H_

#include <vmmlib/vector.hpp>

#include <memory>

namespace iv
{

/*
 * DataHandler
 */

// File types supported
#define IV_FILE_TYPE_UNKNOWN    0
#define IV_FILE_TYPE_HDF5       1

typedef unsigned file_type_t;
typedef std::vector< std::string > file_args_t;

typedef unsigned plane_id_t;

/*
 * Other
 */

/* indentifier type for node */
typedef unsigned long long index_node_t;

typedef vmml::vector< 3 , int32_t > vec3int32_t;
typedef vmml::vector< 2 , int32_t > vec2int32_t;

}

#endif // _IV_TYPES_H_
