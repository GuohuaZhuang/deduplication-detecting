/*
 * util code
 * 
 * @filename: util.h
 * @project: deduplication
 * @abstract: die, get_file, and so on.
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_UTIL_H_
#define _DEDUPLICATION_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <limits.h>

void die(int ex, const char *fmt, ...);

void get_file(char *file_name, char **data, long *file_length);

/**
 * Here's a version of the function that does not suffer from reentrancy 
 * issues or limits on the size/type of the argument:
 */

#define FMT_BUF_SIZE (CHAR_BIT*sizeof(uintmax_t)+1)

extern char g_tmp_64bit_string[FMT_BUF_SIZE];

extern char* binary_fmt(uintmax_t x, char buf[FMT_BUF_SIZE]);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_UTIL_H_ */
