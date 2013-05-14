/**
 * Implementation of the M. Charikar's document similarity hash.
 * 
 * @filename: lookup3.h
 * @project: deduplication
 * @abstract: Charikar's simhash and Google's hamming dist.
 * 
 * @protocol: Public domain
 * @version: 2.0
 * @author: Bob Jenkins, Firstboy
 * @date: 2006-05, 2011-10
 */

#ifndef _DEDUPLICATION_LOOKUP3_H_
#define _DEDUPLICATION_LOOKUP3_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

uint32_t hashword( const uint32_t *k, size_t length, uint32_t initval); 
void hashword2 ( const uint32_t *k, size_t length, uint32_t *pc, uint32_t *pb);   
uint32_t hashlittle( const void *key, size_t length, uint32_t initval);
void hashlittle2( const void *key, size_t length, uint32_t *pc, uint32_t *pb);
uint32_t hashbig( const void *key, size_t length, uint32_t initval);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_LOOKUP3_H_ */
