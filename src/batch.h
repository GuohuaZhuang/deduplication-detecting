/*
 * These duplication batch insert simhash api.
 * 
 * @filename: batch.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 *
 * Usage	:

		// use (12 + 13) bit and simhash_split_12_13_1 split function
		PSimhashIndex index_table = dup_batch_init(simhash_split_12_13_1, (12 + 13));
		
		if (0 == index_table)
		{
			// print error
		}
		
		const char* text_content = "this is text content.";
		if (dup_batch_insert_text_simhash(
			index_table, 
			text_content, 
			strlen(text_content)) )
		{
			// print error
		}
		
		if (dup_batch_release(index_table, "./index_table.dup"))
		{
			// print error
		}

 * Compile	: gcc [your code file] -ldup
 */


#ifndef _DEDUPLICATION_BATCH_H_
#define _DEDUPLICATION_BATCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * @brief dup local batch init.
 * @return return NULL if error occur, else return index_table handle.
 */
PSimhashIndex dup_batch_init(
	f_simhash_split_method simhash_split, 
	int bit);

/**
 * @brief dup client search simhash by text.
 * @param index_table simhash index table handle.
 * @param text [in] search text as key.
 * @param length [in] search text length.
 * @return return 0 if success, else non-zero.
 */
int dup_batch_insert_text_simhash(
	PSimhashIndex index_table, 
	const char* text, 
	const int length);

/**
 * @brief dup local batch release.
 * @param index_table simhash index table handle.
 * @param index_file_path save index table filepath.
 * @return return 0 if success, else non-zero.
 */
int dup_batch_release(
	PSimhashIndex index_table, 
	const char* index_file_path);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_BATCH_H_ */

