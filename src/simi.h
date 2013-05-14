/**
 * Implementation of the M. Charikar's document similarity hash.
 * FAST 64bit version with no word stemming, stop words, token weights.
 * 
 * @filename: simi.h
 * @project: deduplication
 * @abstract: Charikar's simhash and Google's hamming dist.
 * 
 * @protocol: Public domain
 * @version: 2.0
 * @author: Viliam Holub, Firstboy
 * @date: 2011-10-18
 */

#ifndef _DEDUPLICATION_SIMI_H_
#define _DEDUPLICATION_SIMI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

int hamming_dist( uint64_t a1, uint64_t a2);
uint64_t charikar_hash64( const char *data, long data_length);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_SIMI_H_ */
