/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_TESTFILEHANDLER_H_
#define _IV_TESTFILEHANDLER_H_

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileHandler.h>

#include <hdf5.h>

namespace iv
{

namespace DataHandler
{

class  TestFileHandler : public FileHandler
{
friend FactoryFileHandler;
private:
    TestFileHandler() {}

public:
    TestFileHandler(const TestFileHandler&) = delete; // deleted
    TestFileHandler& operator = (const TestFileHandler&) = delete; // deleted

    ~TestFileHandler() {}

    virtual bool init( const file_args_t& file_params );

    virtual const float * getxGrid( ) const;
    virtual const float * getyGrid( ) const;
    virtual const float * getzGrid( ) const;

    virtual void read( float * const data,
                       const vec3int32_t& start,
                       const vec3int32_t& end
                     ) const;

private:
    virtual void _close( );

    std::unique_ptr<float[]> _data;
    std::unique_ptr<float[]> _grid;

};

}

}

#endif // _IV_TESTFILEHANDLER_H_
#define _IV_TESTFILEHANDLER_H_
