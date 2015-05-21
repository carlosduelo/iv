/*
Author: Carlos Duelo Serrano <cduelo@cesvima.upm.es>
Company: Cesvima, UPM

Notes:

 */

#include <iv/dataHandler/cache/cacheStats.h>

namespace iv
{

namespace DataHandler
{

CacheStatsPtr& CacheStats::getInstance()
{
    static CacheStatsPtr instance( new CacheStats() );
    return instance;
}

void CacheStats::newStat( const level_t level, const index_node_t id,
                          const uint32_t misses, const uint32_t hits,
                          const std::shared_ptr< std::vector< std::chrono::nanoseconds >  >& resolveTime )
{
    table_ptr& table = _tables[ level ];
    if( !table )
        table.reset( new table_t ); 
    StatsPtr& stats = (*table)[ id ];
    if( !stats )
    {
        stats.reset( new Stats( misses, hits, resolveTime ) );
    }
    else
    {
    }
}

void CacheStats::printStats()
{
    for( auto t = _tables.begin(); t != _tables.end(); ++ t )
    {
        std::cout << "Level " << t->first << std::endl;
        for( auto c = t->second->begin(); c != t->second->end(); ++c )
        {
            const std::shared_ptr< std::vector< std::chrono::nanoseconds > >& v = c->second->_resolveTime;
            std::cout << "\t" << c->first << "\t"
                      << "misses " << c->second->_cacheMisses << " "
                      << "hits " << c->second->_cacheHits << " "
                      << "times ";
            for( auto i = v->begin(); i != v->end(); ++i )
                std::cout << i->count() << " ";
            std::cout << std::endl;
        }
}
}
    

}

}
