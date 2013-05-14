/*
 * These duplication's data structure and choice functions.
 * 
 * @filename: data.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_DATA_H_
#define _DEDUPLICATION_DATA_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

typedef size_t		dup_index_t;				// array index type

typedef dup_index_t (*f_simhash_split_method)(uint64_t hashcode);	// simhash split

typedef struct _simhash_node
{
	uint64_t				hashcode;		// simhash code 64bit
	struct _simhash_node*	next;			// node next point
}
SimhashNode, *PSimhashNode;

typedef struct _simhash_index
{
	struct _simhash_node**	table;			// simhash node table
	f_simhash_split_method	simhash_split;	// simhash split function
	int						bit;			// index bit
	uint64_t				count;			// total simhash code count
}
SimhashIndex, *PSimhashIndex;

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_DATA_H_ */

