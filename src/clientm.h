/*
 * These duplication's socket client side api.
 * 
 * @filename: clientm.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 *
 * Usage	:
 
		dup_clientm_init();
		
		uint64_t hashcode = 0;
	
		uint64_t* simhashs = NULL;
		uint64_t size = 0;
		if (0 != dup_clientm_search_simhash(hashcode, &simhashs, &size, 1))
		{
			// print error
		}
		
		if (0 != dup_clientm_delete_simhash(hashcode))
		{
			// print error
		}
		
		// print result
		fprintf(stdout, "[INFO] -- result simhash count = %lld\n", size);
		int i = 0;
		for (i = 0; i < size; i ++)
		{
			fprintf(stdout, "[INFO] -- result simhash is(hex): [%016llx]\n", simhashs[i]);
		}
		free(simhashs);
		simhashs = NULL;
		
		dup_clientm_release();

 * Compile	: gcc [your code file] -ldup
 */


#ifndef _DEDUPLICATION_CLIENTM_H_
#define _DEDUPLICATION_CLIENTM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"

/**
 * @brief dup client site init.
 * @return return 0 if success, else non-zero.
 */
int dup_clientm_init(void);

/**
 * @brief dup client search simhash by text.
 * @param text [in] search text as key.
 * @param length [in] search text length.
 * @param p_hashcode [out] text's hashcode.
 * @return return 0 if success, else non-zero.
 */
int dup_clientm_get_text_hashcode(
	const char* text, 
	const int length, 
	uint64_t* p_hashcode 
	);

/**
 * @brief dup client search simhash by hashcode.
 * @param hashcode [in] search hashsode as key.
 * @param simhashs [out] finded simhashs array.
 * @param size [out] finded simhashs array size.
 * @param f_insert [in] whether to insert search hashcode.
 * @return return 0 if success, else non-zero.
 */
int dup_clientm_search_simhash(
	uint64_t hashcode, 
	uint64_t** simhashs, 
	uint64_t* size, 
	int f_insert
	);

/**
 * @brief dup client search simhash by text.
 * @param text [in] search text as key.
 * @param length [in] search text length.
 * @param simhashs [out] finded simhashs array.
 * @param size [out] finded simhashs array size.
 * @return return 0 if success, else non-zero.
 */
int dup_clientm_search_text_simhash(
	const char* text, 
	const int length, 
	uint64_t** simhashs, 
	uint64_t* size
	);

/**
 * @brief dup client delete simhash use hashcode.
 * @param hashcode [in] search hashsode as key.
 * @return return 0 if success, else non-zero.
 */
int dup_clientm_delete_simhash(
	uint64_t hashcode
	);

/**
 * @brief dup client site release.
 * @return return 0 if success, else non-zero.
 */
int dup_clientm_release(void);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_CLIENTM_H_ */

