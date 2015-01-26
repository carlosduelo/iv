/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OBJECT_HANDLER_H_
#define _IV_OBJECT_HANDLER_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

namespace iv
{

namespace DataHandler
{

class ObjectHandler
{
public:
    ~ObjectHandler();

    const float* try_lock();

    const float* lock();

    index_node_t getID() const;

private:
    friend class CacheObject;
    ObjectHandler( const CacheObjectPtr& obj);

    void _unlock();

    const CacheObjectPtr   _obj;
    bool                   _locked;
    const float*           _data;
};

}

}

#endif // _IV_OBJECT_HANDLER_H_
