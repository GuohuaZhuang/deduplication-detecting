/*
 * hashmap.c
 * Generic map implementation. This class is not thread-safe.
 * free() must be invoked when only one thread has access to the hashmap.
 *  Created on: April 7th, 2005
 *  Modify on :	Nov. 3th, 2011
 *      Author: Elliott Back, Firstboy
 */

#include "hashmap.h"
#include "util.h"
#include "define.h"
#include "lookup3.h"
#include <stdlib.h>
#include <stdio.h>

#define HASHMAP_INITIAL_SIZE 1024

// We need to keep keys and values.
typedef struct _hashmap_element
{
	char* skey;
	int key;
	int in_use;
	any_t data;
}
hashmap_element;

// A hashmap has some maxinum size and current size,
// as well as the data to hold.
typedef struct _hashmap_map
{
	int table_size;
	int size;
	hashmap_element* data;
}
hashmap_map;

/*
 * Return an empty hashmap, or NULL no failure.
 */
map_t hashmap_new()
{
	hashmap_map* m = (hashmap_map*) malloc(sizeof(hashmap_map));
	if (!m)
	{
		die(-1, "[ERROR] -- %s:%s hashmap_new failed.\n", __FILE__, __LINE__);
	}
	
	m->data = (hashmap_element*) calloc(HASHMAP_INITIAL_SIZE, sizeof(hashmap_element));
	if (!m->data)
	{
		free(m);
		m = NULL;
		die(-1, "[ERROR] -- %s:%s hashmap_new failed.\n", __FILE__, __LINE__);
	}
	
	// HASHMAP_INITIAL_SIZE * sizeof(hashmap_element) is not overflow.
	memset(m->data, 0, HASHMAP_INITIAL_SIZE * sizeof(hashmap_element) );
	
	m->table_size = HASHMAP_INITIAL_SIZE;
	m->size = 0;
	
	return m;
}

/*
 * Deallocate the hashmap.
 */
void hashmap_free(map_t in)
{
	hashmap_map* m = (hashmap_map*) in;
	
	if (m && m->data)
	{
		int i = 0;
		for (; i < m->table_size; i ++)
		{
			if (m->data[i].skey)
			{
				free(m->data[i].skey);
				m->data[i].skey = NULL;
			}
		}
	}
	
	free(m->data);
	m->data = NULL;
	
	// Following crashes which said by first author, but I want to try.
	free(m);
	m = NULL;
}

/*
 * Hashing function for an integer
 */
unsigned int hashmap_hash_int(hashmap_map* m, unsigned int key)
{
	// Robert Jenkins' 32 bit Mix Function.
	key += (key << 12);
	key ^= (key >> 22);
	key += (key << 4);
	key ^= (key >> 9);
	key += (key << 10);
	key ^= (key >> 2);
	key += (key << 7);
	key ^= (key >> 12);

//	// Knuth's Multiplicative Method.
//	key = (key >> 3) * 2654435761;

	return key % m->table_size;
}

/*
 * Return the integer of the location in data
 * to store the point to the item, or MAP_FULL.
 */
int hashmap_hash(map_t in, int key)
{
	int curr;
	int i;
	
	// Cast the hashmap.
	hashmap_map* m = (hashmap_map*) in;
	
	// If full, return immediately.
	if (m->size == m->table_size) return MAP_FULL;
	
	// Find the best index.
	curr = hashmap_hash_int(m, key);
	
	// Linear probing.
	for (i = 0; i < m->table_size; i++)
	{
		if (m->data[curr].in_use == 0)
			return curr;
		if (m->data[curr].key == key && m->data[curr].in_use == 1)
			return curr;
		curr = (curr + 1) % m->table_size;
	}
	
	return MAP_FULL;
}

/*
 * Double the size of the hashmap, and rehashes all the elements
 */
int hashmap_rehash(map_t in)
{
	int i;
	int old_size;
	hashmap_element* curr;
	
	// Setup the new elements.
	hashmap_map* m = (hashmap_map*) in;
	
	// Overflow check.
	size_t block_size = 2 * m->table_size;
	if (block_size < m->table_size)
	{
		die(-1, "[ERROR] -- %s:%s hashmap rehash is overflow failed.\n", __FILE__, __LINE__);
	}
	
	hashmap_element* temp = (hashmap_element*) calloc(2 * m->table_size, sizeof(hashmap_element));
	if (!temp) return MAP_OMEM;
	
	// Update the array.
	curr = m->data;
	m->data = temp;
	
	// Update the size.
	old_size = m->table_size;
	m->table_size = 2 * m->table_size;
	m->size = 0;
	
	// Rehash the elements.
	for (i = 0; i < old_size; i++)
	{
		int status = hashmap_put(m, curr[i].skey, strlen(curr[i].skey), curr[i].data);
		if (status != MAP_OK)
			return status;
	}
	
	free(curr);
	
	return MAP_OK;
}

/*
 * Add a pointer to the hashmap with some key.
 */
int hashmap_put(map_t in, const char* skey, size_t slen, any_t value)
{
	int index;
	hashmap_map* m;

	// Cast the hashmap.
	m = (hashmap_map*) in;

	// Find a place to put our value.
	int key = hashlittle(skey, slen, 0);
	
	index = hashmap_hash(in, key);
	while (index == MAP_FULL)
	{
		if (hashmap_rehash(in) == MAP_OMEM)
		{
			return MAP_OMEM;
		}
		index = hashmap_hash(in, key);
	}
	
	// Set the data.
	m->data[index].data = value;
	m->data[index].key = key;
	
	if (m->data[index].skey)
	{
		free(m->data[index].skey);
		m->data[index].skey = NULL;
	}
	m->data[index].skey = (char*) malloc(slen + 1);
	memcpy(m->data[index].skey, skey, slen);
	
	m->data[index].skey[slen] = 0;
	m->data[index].in_use = 1;
	m->size++;
	
	return MAP_OK;
}

/*
 * Get your pointer out of the hashmap with a key.
 */
int hashmap_is_exist(map_t in, const char* skey, size_t slen)
{
	int curr;
	int i;
	hashmap_map* m;
	
	// Cast the hashmap.
	m = (hashmap_map*) in;
	if (!m) return MAP_MISSING;
	
	// Find data localtion.
	int key = hashlittle(skey, slen, 0);
	curr = hashmap_hash_int(m, key);
	
	// Linear probing, if necessary.
	for (i = 0; i < m->table_size; i++)
	{
		if(	m->data[curr].key == key 
			&& m->data[curr].in_use == 1 
			&& (0 == strncmp(skey, m->data[curr].skey, slen)) )
		{
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}
	
	return MAP_MISSING;
}

/*
 * Get your pointer out of the hashmap with a key.
 */
int hashmap_get(map_t in, const char* skey, size_t slen, any_t* arg)
{
	int curr;
	int i;
	hashmap_map* m;
	
	// Cast the hashmap.
	m = (hashmap_map*) in;
	
	// Find data localtion.
	int key = hashlittle(skey, slen, 0);
	curr = hashmap_hash_int(m, key);
	
	// Linear probing, if necessary.
	for (i = 0; i < m->table_size; i++)
	{
		if(m->data[curr].key == key && m->data[curr].in_use == 1)
		{
			*arg = (any_t) (m->data[curr].data);
			return MAP_OK;
		}
		curr = (curr + 1) % m->table_size;
	}
	
	*arg = NULL;
	
	return MAP_MISSING;
}

/*
 * Return the length of the hashmap.
 */
int hashmap_length(map_t in)
{
	hashmap_map* m = (hashmap_map*) in;
	if(m != NULL) return m->size;
	else return 0;
}

