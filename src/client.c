/**
 * Some recommond here:
 * 
 * int inet_pton(int af, const char *src, void *dst);
 * 
 * inet_pton() returns 1 on success (network address was successfully converted). 
 * 0 is returned if src does not contain a character string representing a valid 
 * network address in the specified address family. If af does not contain a 
 * valid address family, -1 is returned and errno is set to EAFNOSUPPORT.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "common.h"

/// #define SOCKET_CLIENT_MAIN_TEST
#ifdef SOCKET_CLIENT_MAIN_TEST
int main(int argc, char* argv[])
{
	dup_client();
	return 0;
}
#endif // SOCKET_CLIENT_MAIN_TEST


int dup_client(const char* host, 
	int port, 
	f_simhash_split_method	simhash_split, 
	uint64_t hashcode, 
	PSimhashNode* p_phashcode, 
	int* p_count, 
	uint64_t hashflag)
{
	(*p_count) = 0;
	(*p_phashcode) = 0;
	
	int socket_fd;						// connect socket fd
	int server_port = port;				// server port
	
	// init socket
	if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "[ERROR] -- dup client open socket failed.\n");
		return -1;
	}
	struct timeval tv;
	tv.tv_sec = DISTRUBUTE_SOCKET_TIME_OUT_SECOND;
	tv.tv_usec = DISTRUBUTE_SOCKET_TIME_OUT_USECOND;
	setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
	
	// connect server
	struct sockaddr_in server_address;
	memset(&server_address, '0', sizeof(server_address));
	
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	
	if (inet_pton(AF_INET, host, &server_address.sin_addr) <= 0)
	{
		fprintf(stderr, "[ERROR] -- dup client inet_pton error occured.\n");
		return -1;
	}
	
	#define _TRY_COUNT_INNER_CODE											\
	if (n_try_count <= 0)													\
	{																		\
		return -1;															\
	}																		\
	else																	\
	{																		\
		usleep((DISTRUBUTE_SOCKET_CONN_TRY_COUNT - n_try_count) * 100000);	\
	}
	
	int n_try_count = DISTRUBUTE_SOCKET_CONN_TRY_COUNT;
	while (connect(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)))
	{
		n_try_count --;
		fprintf(stderr, "[ERROR] -- dup client connect error occured (try times: %d).\n", 
			DISTRUBUTE_SOCKET_CONN_TRY_COUNT - n_try_count);
		_TRY_COUNT_INNER_CODE
	}
	
	// read or write interaction
	size_t n = 0;
	
	n_try_count = DISTRUBUTE_SOCKET_CONN_TRY_COUNT;
	while (write(socket_fd, &hashflag, sizeof(uint64_t)) < 0)
	{
		fprintf(stderr, "[ERROR] -- dup client write hashflag error.\n");
		_TRY_COUNT_INNER_CODE
	}
	
	fprintf(stdout, "[INFO] -- client write a hashflag is(hex): [%016llx]\n", hashflag);
	
	while (write(socket_fd, &hashcode, sizeof(uint64_t)) < 0)
	{
		fprintf(stderr, "[ERROR] -- dup client write hashcode error.\n");
		_TRY_COUNT_INNER_CODE
	}
	
	fprintf(stdout, "[INFO] -- client write a simhash is(hex): [%016llx]\n", hashcode);
	
	while ((n = read(socket_fd, &hashcode, sizeof(uint64_t))) > 0)
	{
		PSimhashNode p_tmp_node_sh = (PSimhashNode) malloc(sizeof(SimhashNode));
		p_tmp_node_sh->hashcode = hashcode;
		p_tmp_node_sh->next = (*p_phashcode);
		(*p_phashcode) = p_tmp_node_sh;
		(*p_count) ++;
		
		fprintf(stdout, "[INFO] -- client read a simhash is(hex): [%016llx]\n", hashcode);
	}
	
	if (close(socket_fd))
	{
		fprintf(stderr, "[ERROR] -- dup client close socket error.\n");
		return -1;
	}
	
	return 0;
}

