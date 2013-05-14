/*
 * These duplication's data structure and choice functions.
 * 
 * @filename: dup.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_DUP_H_
#define _DEDUPLICATION_DUP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "data.h"

/**
 * Create simhash index table.
 * @param bit index bit count.
 * @param simhash_split simhash_split function.
 * @return point of simhash index table.
 */
PSimhashIndex create_simhash_index(
		int bit, 
		f_simhash_split_method simhash_split
	);

/**
 * Release simhash index table.
 * @param shindex simhash index table.
 * @return return 0 if success, else failed.
 */
int release_simhash_index(
		PSimhashIndex shindex
	);

/**
 * Read simhash index table from file.
 * @param shindex simhash index table.
 * @param file_path simhash index file path.
 * @return return 0 if success, else failed.
 */
int read_from_file(
		PSimhashIndex shindex, 
		const char* file_path
	);

/**
 * Write simhash index table to file.
 * @param shindex simhash index table.
 * @param file_path simhash index file path.
 * @param is_lock is lock file to write.
 * @return return 0 if success, else failed.
 */
int write_to_file(
		PSimhashIndex shindex, 
		const char* file_path, 
		int is_lock
	);

/**
 * Insert simhash to index table.
 * @param shindex simhash index table.
 * @param hashcode simhash code.
 * @return return 0 if success, else failed.
 */
int insert_simhash(
		PSimhashIndex shindex, 
		uint64_t hashcode
	);

/**
 * Search and insert simhash to index table.
 * @param shindex simhash index table.
 * @param hashcode simhash code.
 * @param p_phashcode fined similar simhash codes.
 * @param p_count fined similar simhash codes count.
 * @return return 0 if success, else failed.
 */
int search_and_insert_simhash(
		PSimhashIndex shindex, 
		uint64_t hashcode, 
		PSimhashNode* p_phashcode, 
		int* p_count, 
		int f_insert
	);

/**
 * Delete simhash from index table.
 * @param shindex simhash index table.
 * @param hashcode simhash code.
 * @return return 0 if success, else failed.
 */
int delete_simhash(
	PSimhashIndex shindex, 
	uint64_t hashcode);

/**
 * Insert text which convert to simhash to index table.
 * @param shindex simhash index table.
 * @param text text to be insert.
 * @param length text length.
 * @return return 0 if success, else failed.
 */
int insert_text_simhash(
		PSimhashIndex shindex, 
		char* text, 
		long length
	);

/**
 * Search and insert text convert to simhash to index table.
 * @param shindex simhash index table.
 * @param text text to be insert.
 * @param length text length.
 * @param p_phashcode fined similar simhash codes.
 * @param p_count fined similar simhash codes count.
 * @return return 0 if success, else failed.
 */
int search_and_insert_text_simhash(
		PSimhashIndex shindex, 
		char* text, 
		long length, 
		PSimhashNode* p_phashcode, 
		int* p_count
	);

/**
 * Insert text which convert to simhash wide to index table.
 * @param shindex simhash index table.
 * @param text text to be insert.
 * @param length text length.
 * @param
 * @return return 0 if success, else failed.
 */
int insert_text_simhashw(
		PSimhashIndex shindex, 
		char* text, 
		long length, 
		int stoken_size
	);

/**
 * Search and insert text convert to simhash wide to index table.
 * @param shindex simhash index table.
 * @param text text to be insert.
 * @param length text length.
 * @param p_phashcode fined similar simhash codes.
 * @param p_count fined similar simhash codes count.
 * @return return 0 if success, else failed.
 */
int search_and_insert_text_simhashw(
		PSimhashIndex shindex, 
		char* text, 
		long length, 
		int stoken_size, 
		PSimhashNode* p_phashcode, 
		int* p_count
	);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_DUP_H_ */
