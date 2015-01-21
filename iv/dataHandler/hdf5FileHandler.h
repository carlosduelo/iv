/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_HDF5FILEHANDLER_H_
#define _IV_HDF5FILEHANDLER_H_

#include <iv/dataHandler/types.h>
#include <iv/dataHandler/fileHandler.h>

#include <hdf5.h>

namespace iv
{

namespace DataHandler
{

class  HDF5FileHandler : public FileHandler
{
friend FactoryFileHandler;
private:
    HDF5FileHandler() :
            FileHandler(),
            _file_id( 0 ),
            _dataset_id( 0 ),
            _spaceid( 0 ),
            _ndim( 0 ),
            _datatype( 0 )
    {
    }

public:
    HDF5FileHandler(const HDF5FileHandler&) = delete; // deleted
    HDF5FileHandler& operator = (const HDF5FileHandler&) = delete; // deleted

    ~HDF5FileHandler() {}

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

    /*
     * HDF5 stuff
     */
    hid_t           _file_id;
    hid_t           _dataset_id;
    hid_t           _spaceid;
    int             _ndim;
    hsize_t         _dims[3];
    hid_t			_datatype;

    /*
     * Grid vectors
     */
    std::unique_ptr<float[]> _xGrid;
    std::unique_ptr<float[]> _yGrid;
    std::unique_ptr<float[]> _zGrid;

    bool _readGrid( const std::string& name,
                    float * const grid,
                    const hsize_t dim );
};

}

}

#endif // _IV_FILEHANDLER_H_
