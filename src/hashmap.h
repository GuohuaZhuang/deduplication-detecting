/**
 * Implementation of the Elliott Back hashmap.
 * 
 * @filename: hashmap.h
 * @project: deduplication
 * @abstract: hashmap for C program.
 * 
 * @protocol: Public domain
 * @version: 2.0
 * @author: Elliott Back, Firstboy
 * @date: 2011-11-03
 */

#ifndef _DEDUPLICATION_HASHMAP_H_
#define _DEDUPLICATION_HASHMAP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#define MAP_MISSING		-3	// No such element
#define MAP_FULL		-2	// Hashmap is full
#define MAP_OMEM		-1	// Out of Memory
#define MAP_OK			0	// OK


typedef void* any_t;

typedef int (*PFany)(any_t, any_t);

typedef any_t map_t;


extern map_t hashmap_new();

extern void hashmap_free(map_t in);

extern int hashmap_put(map_t in, const char* skey, size_t slen, any_t value);

extern int hashmap_get(map_t in, const char* skey, size_t slen, any_t* arg);

extern int hashmap_is_exist(map_t in, const char* skey, size_t slen);

extern int hashmap_length(map_t in);


#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_HASHMAP_H_ */
