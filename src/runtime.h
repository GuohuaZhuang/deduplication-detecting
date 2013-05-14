/*
 * runtime head code
 * 
 * @filename: runtime.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_RUNTIME_H_
#define _DEDUPLICATION_RUNTIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

#define	SET_RUN_TIME	struct timeval tpstart,tpend; \
	float timeuse; \
	gettimeofday(&tpstart,NULL);

#define GET_RUN_TIME	gettimeofday(&tpend,NULL); \
	timeuse=1000000 * \
		(tpend.tv_sec-tpstart.tv_sec) + \
		tpend.tv_usec-tpstart.tv_usec; \
	timeuse/=1000000; \
	fprintf(stderr, "Run time: %fs\n", timeuse);

#ifdef __cplusplus
}
#endif

#endif /* _DEDUPLICATION_RUNTIME_H_ */

