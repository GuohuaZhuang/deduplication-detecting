/*
 * conf head code
 * 
 * @filename: conf.h
 * @project: deduplication
 * 
 * @protocol: Public domain
 * @version: 1.0
 * @author: Firstboy
 * @date: 2012-07-20
 */

#ifndef _DEDUPLICATION_CONF_H_
#define _DEFUPLICATION_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "data.h"
#include "split.h"

// similar hash hamming distance. (require configure)
#define SIMILAR_HAMMING_DIST				3
// max child server process
#define MAX_CHILD_PROCESS					10
// max distrubute socket time out (second)
#define DISTRUBUTE_SOCKET_TIME_OUT_SECOND	2
// max distrubute socket time out (usecond)
#define DISTRUBUTE_SOCKET_TIME_OUT_USECOND	0
// max distrubute socket conntect try count
#define DISTRUBUTE_SOCKET_CONN_TRY_COUNT	3

typedef struct _dup_conf_node
{
	char* host;						// host or ip address
	char* port;						// port of service
	char* method;					// method of split simhash to index
}
DupConfNode, *PDupConfNode;

int get_host_port_conf(const char* s, PDupConfNode** p_confnode, int* n_size);

f_simhash_split_method get_simhash_split_method(const char* s);

int get_simhash_split_bit(const char* s);

extern char g_simhash_table_filepath[10][1024];

char* get_simhash_table_filepath(const char* s);

#ifdef __cplusplus
}
#endif

#endif /* _DEFUPLICATION_CONF_H_ */

