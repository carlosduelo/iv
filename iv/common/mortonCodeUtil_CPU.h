/*
Author: Carlos Duelo Serrano
Company: Cesvima

Notes:

 */

#ifndef EQ_MIVT_MORTON_CPU_H
#define EQ_MIVT_MORTON_CPU_H

#include <iv/common/types.h>

namespace iv
{

inline index_node_t dilateInteger( index_node_t x )
{
	x = ( x | ( x << 20 ) ) & 0x000001FFC00003FF;
	x = ( x | ( x << 10 ) ) & 0x0007E007C00F801F;
	x = ( x | ( x << 4 ) )  & 0x00786070C0E181C3;
	x = ( x | ( x << 2 ) )  & 0x0199219243248649;
	x = ( x | ( x << 2 ) )  & 0x0649249249249249;
	x = ( x | ( x << 2 ) )  & 0x1249249249249249;
	return x;
}

inline uint32_t getIndexLevel( const index_node_t index)
{
	if( index >= 144115188075855872 )
		return 19;
	else if( index >= 18014398509481984 )
		return 18;
	else if( index >= 2251799813685248 )
		return 17;
	else if( index >= 281474976710656 )
		return 16;
	else if( index >= 35184372088832 )
		return 15;
	else if( index >= 4398046511104 )
		return 14;
	else if( index >= 549755813888 )
		return 13;
	else if( index >= 68719476736 )
		return 12;
	else if( index >= 8589934592 )
		return 11;
	else if( index >= 1073741824 )
		return 10;
	else if( index >= 134217728 )
		return 9;
	else if( index >= 16777216 )
		return 8;
	else if( index >= 2097152 )
		return 7;
	else if( index >= 262144 )
		return 6;
	else if( index >= 32768 )
		return 5;
	else if( index >= 4096 )
		return 4;
	else if( index >= 512 )
		return 3;
	else if( index >= 64 )
		return 2;
	else if( index >= 8 )
		return 1;
	else if( index == 1 )
		return 0;
	else
		return -1;
}

inline vec3int32_t getMinBoxIndex( index_node_t index, uint32_t& level,
                                                        const uint32_t nLevels )
{
	vec3int32_t Box( 0,0,0 );
	level = getIndexLevel( index );

	if( index == 1 )
		return Box; // minBOX (0,0,0) and level 0

	for( uint32_t l = level; l > 0; l-- )
	{
		Box[2] +=  ( index & 0x1 ) << ( nLevels -l ); index >>= 1;
		Box[1] +=  ( index & 0x1 ) << ( nLevels -l ); index >>= 1;
		Box[0] +=  ( index & 0x1 ) << ( nLevels -l ); index >>= 1;
	}

	return Box;
}

inline vec3int32_t getMinBoxIndex2( index_node_t index, const uint32_t level,
                                        const uint32_t nLevels )
{
	vec3int32_t minBox( 0,0,0 );

	if( index == 1 )
		return minBox;// minBOX (0,0,0) and level 0

	for( uint32_t l = level; l > 0; l-- )
	{
		minBox[2] +=  ( index & 0x1 ) << ( nLevels - l ); index >>= 1;
		minBox[1] +=  ( index & 0x1 ) << ( nLevels - l ); index >>= 1;
		minBox[0] +=  ( index & 0x1 ) << ( nLevels - l ); index >>= 1;
	}

	return minBox;
}

inline  index_node_t coordinateToIndex( const vec3int32_t& pos,
                                        const uint32_t level,
                                        const int nLevels)
{
	if( level == 0 )
		return 1;

	index_node_t code 	= ( index_node_t )1 << ( nLevels * 3 );
	code 	|= dilateInteger( ( index_node_t ) pos.x() ) << 2;
	code 	|= dilateInteger( ( index_node_t ) pos.y() ) << 1;
	code 	|= dilateInteger( ( index_node_t ) pos.z() );

	code >>= ( nLevels - level ) * 3;

	return code;
}

}
#endif
