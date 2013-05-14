/**
 * Some recommond here:
 * 
 * int listen(int sockfd, int backlog);
 * 
 * If the backlog argument is greater than the value in 
 * /proc/sys/net/core/somaxconn, then it is silently truncated to that value; 
 * the default value in this file is 128. In kernels before 2.4.25, this limit 
 * was a hard coded value, SOMAXCONN, with the value 128.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include "common.h"

#define DUP_MAX_SOCKET_CONN 128
// #define DUPD_INFO
// #define DUPD_RTIME

PSimhashIndex index_table = NULL;
char g_index_file_path[1024] = {0};

/// #define SOCKET_SERVER_MAIN_TEST
#ifdef SOCKET_SERVER_MAIN_TEST
int main(int argc, char* argv[])
{
	dup_server();
	return 0;
}
#endif // SOCKET_SERVER_MAIN_TEST

// stop dup server to save hash table
void stop_dup_server(int dunno)
{
	switch (dunno)
	{
	case 1:
		printf("Get a signal -- SIGHUP\n");
	case 2:
		printf("Get a signal -- SIGINT\n");
	case 3:
		printf("Get a signal -- SIGQUIT\n");
	case 9:
		printf("Get a signal -- SIGKILL\n");
		
		// save hash table before stop dup server
		write_to_file(index_table, g_index_file_path, 1);
		_exit(0);
		
		break;
	default:
		break;
	}
	fprintf(stdout, "[INFO] -- Get a signal (%d).\n", dunno);
	
	return;
}

// save hash table thread
void* save_hash_table_thread(void* v)
{
	write_to_file(index_table, g_index_file_path, 1);
	return NULL;
}

// save hash table
void save_hash_table(int signo)
{
	pthread_t id;
	if (0 != pthread_create(&id, NULL, save_hash_table_thread, NULL))
	{
		fprintf(stderr, "[ERROR] -- dup server create pthread in save_hash_table() failed!\n");
	}
}

// signal process
void init_sigaction(void)
{
	struct sigaction tact;
	tact.sa_handler = save_hash_table;
	tact.sa_flags = 0;
	sigemptyset(&tact.sa_mask);
	sigaction(SIGALRM, &tact, NULL);
}

// time interrupt
void init_time()
{
	// start in 00:00:00 everday
	time_t now_time = time(NULL);
	struct tm* tm_struct = localtime(&now_time);
	
	int hour = tm_struct->tm_hour;
	int min = tm_struct->tm_min;
	int sec = tm_struct->tm_sec;
	int start_sec = (86400 - (hour*60*60 + min*60 + sec));
	
	fprintf(stdout, "[INFO] -- dup server start at %s (start_sec = %d)\n", 
		ctime(&now_time), 
		start_sec);
	
	struct itimerval value;
	value.it_value.tv_sec = start_sec;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 86400;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, NULL);
}


int dup_server(const char* host, 
	int port, 
	f_simhash_split_method simhash_split, 
	int bit, 
	const char* index_file_path)
{
	/*PSimhashIndex */index_table = 
		create_simhash_index(bit, simhash_split);
	
	// Use Random to insert total hashcode.
	// int i = 0;
	
	// Write simhash index table to file.
	if (read_from_file(index_table, index_file_path))
	{
		die(-1, "[ERROR] -- read_from_file simhash table failed.\n");
	}
	
	// set stop server signal
	signal(SIGHUP, stop_dup_server);
	signal(SIGINT, stop_dup_server);
	signal(SIGQUIT, stop_dup_server);
	signal(SIGKILL, stop_dup_server);
	signal(SIGTERM, stop_dup_server);
	
	// set time interrupt invoke to save hash table in disk.
	strcpy(g_index_file_path, index_file_path);
	init_sigaction();
	init_time();
	
	int listen_fd = 0;					// listen handle
	int connect_fd = 0;					// connect a client handle
	int server_port = port;				// server port
	
	// init socket
	if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		fprintf(stderr, "[ERROR] -- dup server open socket failed.\n");
		return -1;
	}
	
	// set reuse fix
	int yes = 1;
	setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&yes, sizeof(yes));
	
	// bind address info
	struct sockaddr_in server_address;	// server address info to be bind
	memset(&server_address, '0', sizeof(server_address));
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(server_port);
	
	if (bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address)) )
	{
		fprintf(stderr, "[ERROR] -- dup server bind error.\n");
		return -1;
	}
	
	// start listen
	if (listen(listen_fd, DUP_MAX_SOCKET_CONN) )
	{
		fprintf(stderr, "[ERROR] -- dup server listen error.\n");
		return -1;
	}
	
	// circle to accept client request
	while (1)
	{
		if ((connect_fd = accept(listen_fd, (struct sockaddr*)NULL, NULL)) < 0)
		{
			fprintf(stderr, "[ERROR] -- dup server accept error.\n");
			continue;
		}
		
		uint64_t hashflag = 0;
		uint64_t hashcode = 0;
		size_t n = 0;
		if ((n = read(connect_fd, &hashflag, sizeof(uint64_t))) < 0)
		{
			fprintf(stderr, "[ERROR] -- dup server read hashflag error.\n");
			continue;
		}
		
		if ((n = read(connect_fd, &hashcode, sizeof(uint64_t))) < 0)
		{
			fprintf(stderr, "[ERROR] -- dup server read hashcode error.\n");
			continue;
		}
		#ifdef DUPD_INFO
		fprintf(stdout, "[INFO] -- server read a simhash is(hex): [%016llx]\n", hashcode);
		#endif // DUPD_INFO
		// process
		#ifdef DUPD_INFO
		fprintf(stdout, "[DEBUG] -- server hashflag = [%016llx]\n", hashflag);
		#endif // DUPD_INFO
		if (0x01 == (hashflag & 0x01))
		{
			// delete hashcode
			if (delete_simhash(index_table, hashcode))
			{
				fprintf(stderr, "[WARNNING] -- delete_simhash failed.\n");
			}
		}
		else
		{
			// search and insert hashcode
			PSimhashNode simhashs = 0;
			int n_ret_count = 0;

			#ifdef DUPD_RTIME
			SET_RUN_TIME
			#endif // DUPD_RTIME
			if (search_and_insert_simhash(index_table, hashcode, &simhashs, &n_ret_count, 
				(0x02 == (hashflag & 0x02) ? 1 : 0) ))
			{
				die(-1, "[ERROR] -- search_and_insert_simhash failed.\n");
			}
			#ifdef DUPD_RTIME
			GET_RUN_TIME
			#endif // DUPD_RTIME

			if (!simhashs)
			{
				#ifdef DUPD_INFO
				fprintf(stdout, "[RESULT] -- simhash is not finded.\n");
				#endif // DUPD_INFO
			}
			else
			{
				#ifdef DUPD_INFO
				fprintf(stdout, "[RESULT] -- simhash is finded (%d) as: \n", n_ret_count);
				#endif // DUPD_INFO
				PSimhashNode p_tmp_sh = simhashs;
	
				while (0 != p_tmp_sh)
				{
					if (write(connect_fd, &(p_tmp_sh->hashcode), sizeof(uint64_t)) < 0)
					{
						fprintf(stderr, "[ERROR] -- dup server write error.\n");
						continue;
					}
					#ifdef DUPD_INFO
					fprintf(stdout, "[INFO] -- server write a simhash is(hex): [%016llx]\n", p_tmp_sh->hashcode);
					#endif // DUPD_INFO
				
					p_tmp_sh = p_tmp_sh->next;
				}
	
				// release finded simhash codes
				while (0 != simhashs)
				{
					p_tmp_sh = simhashs;
					simhashs = p_tmp_sh->next;
					free(p_tmp_sh);
				}
			}
		}
		
		if (close(connect_fd))
		{
			fprintf(stderr, "[ERROR] -- dup server close connect handle error.\n");
			return -1;
		}
	}
	
	if (close(listen_fd))
	{
		fprintf(stderr, "[ERROR] -- dup server close listen handle error.\n");
		return -1;
	}
	
	// service
	// Write simhash index table to file.
	if (write_to_file(index_table, g_index_file_path, 0))
	{
		die(-1, "[ERROR] -- write_to_file simhash table failed.\n");
	}
	if (release_simhash_index(index_table))
	{
		die(-1, "[ERROR] -- release_simhash_index failed.\n");
	}
	
	return 0;
}

