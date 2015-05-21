/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#ifndef _IV_CACHE_STATS_H_
#define _IV_CACHE_STATS_H_

#include <iv/common/types.h>
#include <iv/dataHandler/types.h>

#include <unordered_map>
#include <chrono>

namespace iv
{

namespace DataHandler
{

class CacheStats
{
public:

    static CacheStatsPtr& getInstance();

    void newStat( const level_t level, const index_node_t id,
                  const uint32_t misses, const uint32_t hits,
                  const std::shared_ptr< std::vector< std::chrono::nanoseconds >  >& resolveTime );

    void printStats();
    
private:
    CacheStats()
    {
    }

    class Stats
    {
    public:
        Stats( const uint32_t cacheMisses,
               const uint32_t cacheHits,
               const std::shared_ptr< std::vector< std::chrono::nanoseconds >  >& resolveTime )
            : _cacheMisses( cacheMisses )
            , _cacheHits( cacheHits )
            , _resolveTime( resolveTime )
        {
        }
        const uint32_t _cacheMisses;
        const uint32_t _cacheHits;
        const std::shared_ptr< std::vector< std::chrono::nanoseconds >  > _resolveTime;
    };

    typedef std::unique_ptr< Stats > StatsPtr;
    typedef std::unordered_map< index_node_t, StatsPtr > table_t;
    typedef std::unique_ptr< table_t > table_ptr;
    std::unordered_map< level_t, table_ptr > _tables;

};

}

}

#endif // _IV_CACHE_STATS_H_
