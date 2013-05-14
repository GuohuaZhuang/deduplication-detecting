/*
 * define head code
 * 
 * @filename: define.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_DEFINE_H_
#define _DEFUPLICATION_DEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h> 
#include <unistd.h>
#include <pthread.h>
#include <stddef.h>

#define DUP_STMT_BEGIN do
#define DUP_STMT_END   while(0);

#define dup_return_if_fail(expr) \
DUP_STMT_BEGIN { \
    if (!(expr)) return; \
}DUP_STMT_END

#define dup_return_val_if_fail(expr, val) \
DUP_STMT_BEGIN { \
    if (!(expr)) return val; \
}DUP_STMT_END

#define DUP_MALLOC(object, length) { \
	object = (char*)malloc(length); \
	memset(object, 0, length); \
	};

#define DUP_FREE(object) {if(NULL != object){ \
	free (object); \
	object = NULL; \
	}};

#define DUP_C_STRCPY(object, source) { \
	if (!source || strlen(source) == 0) { object = NULL; } \
	else { \
	int len = strlen(source) + 1; \
	DUP_MALLOC(object, len); \
	strcpy(object, source); \
	} };

#ifdef __cplusplus
}
#endif

#endif /* _DEFUPLICATION_DEFINE_H_ */

