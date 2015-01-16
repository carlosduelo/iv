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

/* indentifier type for node */
typedef unsigned long long index_node_t;

typedef uint32_t    level_t;

typedef vmml::vector< 3 , int32_t > vec3int32_t;
typedef vmml::vector< 2 , int32_t > vec2int32_t;

}

#endif // _IV_TYPES_H_
