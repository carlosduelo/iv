/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_IVDFILEHANDLER_H_
#define _IV_IVDFILEHANDLER_H_

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileReader/fileHandler.h>

namespace iv
{

namespace DataHandler
{

class  IVDFileHandler : public FileHandler
{
friend FactoryFileHandler;
private:
    IVDFileHandler() :
            FileHandler()
    {
    }

public:
    IVDFileHandler(const IVDFileHandler&) = delete; // deleted
    IVDFileHandler& operator = (const IVDFileHandler&) = delete; // deleted

    ~IVDFileHandler() {}

    virtual bool init( const file_args_t& file_params );

    virtual const float * getxGrid( ) const;
    virtual const float * getyGrid( ) const;
    virtual const float * getzGrid( ) const;

    virtual void readCube( float * const data,
                           index_node_t  id,
                           level_t       level,
                           level_t       nLevels,
                           uint32_t      cubeInc
                         ) const;

    virtual void read( float * const data,
                       const vec3int32_t& start,
                       const vec3int32_t& end
                     ) const;

private:
    virtual void _close( );

    char *  _startFile;
    int     _fileD;
    size_t  _lengthFile;

    level_t     _level;
    size_t      _cubeSize;
    uint32_t     _cubeInc;
    int32_t     _numCubes;
    int32_t*    _offsets;
    float *     _cubes;
    index_node_t _idStart;
    index_node_t _idFinish;

    /*
     * Grid vectors
     */
    float* _xGrid;
    float* _yGrid;
    float* _zGrid;
};

}

}

#endif // _IV_IVDFILEHANDLER_H_
