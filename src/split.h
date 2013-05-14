/*
 * These duplication's data structure and choice functions.
 * 64bit, data unique is 24bit, other is 64-24=40bit
 * so, I split 64bit as 12,13,13,13,13

	12,13 ---- C(4,1)=4
	13,13 ---- C(4,2)=6
	total ---- C(5,2)=10

 * Total memory require:

	25bit ---- 2^25*64 = 256MB (other 39bit)
	26bit ---- 2^26*64 = 512MB (other 38bit)

 * So if memory is not big as required, I should use swap 
 * or some storage tech like B-Tree to store.
 * 
 * @filename: split.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_SPLIT_H_
#define _DEDUPLICATION_SPLIT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "data.h"

dup_index_t simhash_split_12_13_1(uint64_t hashcode);	// simhash split 12,13,1
dup_index_t simhash_split_12_13_2(uint64_t hashcode);	// simhash split 12,13,2
dup_index_t simhash_split_12_13_3(uint64_t hashcode);	// simhash split 12,13,3
dup_index_t simhash_split_12_13_4(uint64_t hashcode);	// simhash split 12,13,4
dup_index_t simhash_split_13_13_1(uint64_t hashcode);	// simhash split 13,13,1
dup_index_t simhash_split_13_13_2(uint64_t hashcode);	// simhash split 13,13,2
dup_index_t simhash_split_13_13_3(uint64_t hashcode);	// simhash split 13,13,3
dup_index_t simhash_split_13_13_4(uint64_t hashcode);	// simhash split 13,13,4
dup_index_t simhash_split_13_13_5(uint64_t hashcode);	// simhash split 13,13,5
dup_index_t simhash_split_13_13_6(uint64_t hashcode);	// simhash split 13,13,6

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_SPLIT_H_ */

