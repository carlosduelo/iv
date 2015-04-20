/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_FILEHANDLER_H_
#define _IV_FILEHANDLER_H_

#include <iv/common/types.h>
#include <iv/common/mortonCodeUtil_CPU.h>

namespace iv
{

namespace DataHandler
{

class FactoryFileHandler;

class FileHandler
{
protected:
    FileHandler()
        : _isOpen( false )
        , _realDimension( 0, 0, 0 )
        , _nLevels( 0 )

    {
    }

public:
    FileHandler(const FileHandler&) = delete; // deleted
    FileHandler& operator = (const FileHandler&) = delete; // deleted

    virtual ~FileHandler() {}

    virtual bool init( const file_args_t& file_params ) = 0;

    virtual const float * getxGrid( ) const = 0;
    virtual const float * getyGrid( ) const = 0;
    virtual const float * getzGrid( ) const = 0;

    virtual void readCube( float * const data,
                           index_node_t  id,
                           level_t       level,
                           level_t       nLevels,
                           uint32_t      cubeInc
                         ) const
    {
        const vec3int32_t cubeIncV( cubeInc, cubeInc, cubeInc );
        const int32_t dim = exp2f( nLevels - level );
        const vec3int32_t cubeDimV( dim, dim, dim);
        const vec3int32_t start = getMinBoxIndex2( id, level, nLevels ) - cubeIncV;
        const vec3int32_t end = start + cubeDimV + 2 * cubeIncV;
        read( data, start, end );
    }


    virtual void read( float * const data,
                       const vec3int32_t& start,
                       const vec3int32_t& end
                     ) const = 0;

    const vec3int32_t &getRealDimension() const
    {
        return _realDimension;
    }

    level_t getnLevels() const
    {
        return _nLevels;
    }

    class FileCloser
    {
    public:
        void operator()(FileHandler* f)
        {
            if( f )
                f->close();
            delete f;
        }
    };

protected:

    virtual void _close( ) = 0;

    bool        _isOpen;
    vec3int32_t _realDimension;
    level_t     _nLevels;
private:
    void close( )
    {
        if( _isOpen )
            _close();
    }
};

typedef std::unique_ptr< FileHandler, FileHandler::FileCloser > FileHandlerPtr;
}

}

#endif // _IV_FILEHANDLER_H_
