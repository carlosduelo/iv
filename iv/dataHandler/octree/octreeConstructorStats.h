/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_OCTREE_CONSTRUCTOR_STATS_H_
#define _IV_OCTREE_CONSTRUCTOR_STATS_H_

#include <chrono>
#include <mutex>

namespace iv
{

namespace DataHandler
{

class OctreeConstructorStats
{
public:
    OctreeConstructorStats()
        : _bytesRead( 0 )
        , _cubesComputed( 0 )
        , _readingTime( 0 )
        , _computingTime( 0 )
        , _sortingTime( 0 )
    {
    }

    uint32_t getBytesRead() const { return _bytesRead; }
    uint32_t getCubesComputed() const { return _cubesComputed; }
    std::chrono::milliseconds getReadingTime() const { return _readingTime; }
    std::chrono::milliseconds getComputingTime() const { return _computingTime; }
    std::chrono::milliseconds getSortingTime() const { return _sortingTime; }

    void incReadBytes( const uint32_t bytes )
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        _bytesRead += bytes;
    }

    void incCube( const uint32_t cubes=1 )
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        _cubesComputed += cubes;
    }

    void incReadingTime( const std::chrono::milliseconds time )
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        _readingTime += time;
    }

    void incComputingTime( const std::chrono::milliseconds time )
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        _computingTime += time;
    }

    void incSortingTime( const std::chrono::milliseconds time )
    {
        std::unique_lock< std::mutex > mlock( _mutex );
        _sortingTime += time;
    }

    void print() const
    {
        const float mb = _bytesRead / 1024.0f / 1024.0f;
        const float rS = _readingTime.count() / 1000.0f;
        const float cS = _computingTime.count() / 1000.0f;
        const float sS = _sortingTime.count() / 1000.0f;
        std::cout << "Read: " << mb << " MB" << std::endl;
        std::cout << "Read time: " << rS << " s" << std::endl;
        std::cout << "Read Bandwich " << mb / rS << " MB/s" << std::endl;
        std::cout << "Computing: " << cS << " s" << std::endl;
        std::cout << "Sorting setp: " << sS << " s" << std::endl; 
    }

private:
    uint32_t    _bytesRead;
    uint32_t    _cubesComputed;
    std::chrono::milliseconds       _readingTime;
    std::chrono::milliseconds       _computingTime;
    std::chrono::milliseconds       _sortingTime;
    std::mutex  _mutex;
};

}

}

#endif // _IV_OCTREE_CONSTRUCTOR_STATS_H_
