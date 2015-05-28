/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_QUEUE_H_
#define _IV_QUEUE_H_

#include <queue>
#include <mutex>
#include <condition_variable>

namespace iv
{

namespace DataHandler
{

template< class T >
class Queue
{
public:
    Queue() : _stopped( true ) {}

    void start()
    {
        std::unique_lock< std::mutex > rlock( _mutex );
        _stopped = false;
    }

    void stop()
    {
        std::unique_lock< std::mutex > rlock( _mutex );
        _stopped = true;
        _cond.notify_all();
        std::queue< T > empty;
        std::swap( _queue, empty );
    }

    size_t size()
    {
        std::unique_lock< std::mutex > rlock( _mutex );
        return _queue.size();
    }

    void push( const T& t )
    {
        std::unique_lock< std::mutex > rlock( _mutex );
        if( _stopped )
            return;
        _queue.push( t );
        _cond.notify_one();
    }

    void pop( T& t )
    {
        std::unique_lock< std::mutex > rlock( _mutex );
        while( !_stopped && _queue.empty() )
            _cond.wait( rlock );

        if( _stopped )
            return;

        t = _queue.front();
        _queue.pop();
    }

private:
    bool                    _stopped;
    std::queue< T >         _queue;
    std::mutex              _mutex;
    std::condition_variable _cond;
};

}

}
#endif // _IV_QUEUE_H_
