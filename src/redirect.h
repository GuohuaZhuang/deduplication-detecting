/*
 * redirect head code
 * 
 * @filename: redirect.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_REDIRECT_H_
#define _DEDUPLICATION_REDIRECT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>

#define REDIRECT_STDOUT(_x)  \
	int _old_stdout##_x = dup(1); \
	if (-1 == _old_stdout##_x) { \
		perror( "dup(1) failure.\n" ); \
	} \
	FILE* _new_stdout##_x = fopen("/dev/null", "w"); \
	if (!_new_stdout##_x) { \
		perror( "fopen /dev/null for stdout failure.\n" ); \
	} \
	if (-1 == dup2(fileno(_new_stdout##_x), 1)) { \
		perror( "Can't dup2 stdout" ); \
	}

#define REDUCTION_STDOUT(_x) \
	fflush(stdout); \
	fclose(_new_stdout##_x); \
	dup2(_old_stdout##_x, 1);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_REDIRECT_H_ */

