/*
 * Implementations of the M. Charikar's document similarity hash.
 * 
 * @filename: simiw.h
 * @project: deduplication
 * @abstract: Charikar's simhash and Google's hamming dist.
 * 
 * @protocol: Public domain
 * @version: 2.0
 * @author: Viliam Holub, Firstboy
 * @date: 2011-10-18
 */

#ifndef _DEDUPLICATION_SIMIW_H_
#define _DEDUPLICATION_SIMIW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint64_t charikar_hash64_wide( const char *data, long data_length, int stoken_size);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_SIMIW_H_ */
