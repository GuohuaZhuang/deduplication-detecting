
#include <sys/types.h>
#include <sys/wait.h>
#include "common.h"
#include "server.h"

#define SOCKET_SERVER_MAIN
#ifdef SOCKET_SERVER_MAIN

int main(int argc, char* argv[])
{
	// read configure file
	char *data = NULL;
	long file_length = 0;
	get_file(DUP_SYSCONFIG_DIR"/dup.server.conf", &data, &file_length);
	
	// analysis configure setting
	PDupConfNode* p_confnode = NULL;
	int n_size = 0;
	if (get_host_port_conf(data, &p_confnode, &n_size))
	{
		fprintf(stderr, "[ERROR] -- get_host_port_conf error.\n");
		return -1;
	}
	
	// multi-process
	pid_t cpids[MAX_CHILD_PROCESS] = {0};
	pid_t waits[MAX_CHILD_PROCESS] = {0};
	int statuss[MAX_CHILD_PROCESS] = {0};
	
	int i = 0;
	fprintf(stdout, "[INFO] -- start server count: %d\n", n_size);
	for (i = 0; i < n_size; i ++)
	{
		#ifdef GET_HOST_PORT_CONF_DEBUG
		fprintf(stdout, "p_confnode[%d]->host = [%s]\n", i, p_confnode[i]->host);
		fprintf(stdout, "p_confnode[%d]->port = [%s]\n", i, p_confnode[i]->port);
		fprintf(stdout, "p_confnode[%d]->method = [%s]\n", i, p_confnode[i]->method);
		#endif // GET_HOST_PORT_CONF_DEBUG
		
		cpids[i] = fork();
		if (cpids[i] == -1)
		{
			fprintf(stderr, "[ERROR] -- dup server fork() failed!\n");
			exit(EXIT_FAILURE);
		}
		
		if (cpids[i] == 0)
		{
			fprintf(stdout, "[INFO] -- dup server child PID is %ld\n", 
				(long) getpid());
			if (dup_server(p_confnode[i]->host, 
				atoi(p_confnode[i]->port), 
				get_simhash_split_method(p_confnode[i]->method), 
				get_simhash_split_bit(p_confnode[i]->method), 
				get_simhash_table_filepath(p_confnode[i]->method)) )
			{
				fprintf(stderr, "[ERROR] -- dup server fork() failed!\n");
				_exit(-1);
			}
			_exit(0);
		}
	}
	
	// wait for all child process.
	for (i = 0; i < n_size; i ++)
	{
		do
		{
			waits[i] = waitpid(cpids[i], &(statuss[i]), WUNTRACED | WCONTINUED);
			if (waits[i] == -1)
			{
				fprintf(stderr, "[ERROR] -- dup server waitpid() failed!\n");
				exit(EXIT_FAILURE);
			}
			if (WIFEXITED(statuss[i]))
			{
				fprintf(stdout, "[INFO] -- dup server child process exited, statuss[%d]=%d\n", 
					i, WEXITSTATUS(statuss[i]));
			}
			else if (WIFSIGNALED(statuss[i]))
			{
				fprintf(stdout, "[INFO] -- dup server child process killed by signal %d\n", 
					WTERMSIG(statuss[i]));
			}
			else if (WIFSTOPPED(statuss[i]))
			{
				fprintf(stdout, "[INFO] -- dup server child process stopped by signal %d\n", 
					WSTOPSIG(statuss[i]));
			}
			else if (WIFCONTINUED(statuss[i]))
			{
				fprintf(stdout, "[INFO] -- dup server child process continued\n");
			}
		} while (!WIFEXITED(statuss[i]) && !WIFSIGNALED(statuss[i]));
	}
	
	// free memory here.
	for (i = 0; i < n_size; i ++)
	{
		DUP_FREE(p_confnode[i]->host);
		DUP_FREE(p_confnode[i]->port);
		DUP_FREE(p_confnode[i]->method);
		DUP_FREE(p_confnode[i]);
	}
	DUP_FREE(p_confnode);
	DUP_FREE(data);
	
	return -1;
}

#endif // SOCKET_SERVER_MAIN


