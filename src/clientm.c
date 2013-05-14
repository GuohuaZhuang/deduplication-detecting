
#include "common.h"
#include "client.h"

/// #define GET_HOST_PORT_CONF_DEBUG

// analysis configure setting
PDupConfNode* p_confnode = NULL;
int g_n_confnode_size = 0;

int dup_clientm_init(void)
{
	// read configure file
	char *data = NULL;
	long file_length = 0;
	
	get_file(DUP_SYSCONFIG_DIR"/dup.client.conf", &data, &file_length);
	if (get_host_port_conf(data, &p_confnode, &g_n_confnode_size))
	{
		DUP_FREE(data);
		fprintf(stderr, "[ERROR] -- get_host_port_conf error.\n");
		return -1;
	}
	
	DUP_FREE(data);
	
	return 0;
}

int dup_clientm_get_text_hashcode(
	const char* text, 
	const int length, 
	uint64_t* p_hashcode 
	)
{
	if (0 == p_hashcode)
	{
		fprintf(stderr, "[ERROR] -- dup_clientm_get_text_hashcode must give "\
			"memory but not null point.\n");
		return -1;
	}
	
	(*p_hashcode) = charikar_hash64(text, length);
	
	return 0;
}


typedef struct _clientm_thread_arg
{
	PDupConfNode		pconfnode;
	uint64_t			flag;
	uint64_t			code;
}
ClientmThreadArg, *PClientmThreadArg;

// #define DUP_CLIENTM_PTHREAD_DEBUG

void* dup_clientm_search_simhash_thread(void* v)
{
	PClientmThreadArg p_cta = (PClientmThreadArg)v;
	PDupConfNode p_confnode_i = p_cta->pconfnode;
	uint64_t hashflag = p_cta->flag;
	uint64_t hashcode = p_cta->code;
	
	#ifdef DUP_CLIENTM_PTHREAD_DEBUG
	fprintf(stdout, "thread: p_confnode->host = [%s]\n", p_confnode_i->host);
	fprintf(stdout, "thread: p_confnode->port = [%s]\n", p_confnode_i->port);
	fprintf(stdout, "thread: p_confnode->method = [%s]\n", p_confnode_i->method);
	#endif // DUP_CLIENTM_PTHREAD_DEBUG
	
	PSimhashNode simhashs = 0;
	int n_ret_count = 0;
	
	if (dup_client(p_confnode_i->host, 
		atoi(p_confnode_i->port), 
		get_simhash_split_method(p_confnode_i->method), 
		hashcode, 
		&simhashs, 
		&n_ret_count,  
		hashflag) )
	{
		fprintf(stderr, "[ERROR] -- dup client invoke connect failed!\n");
	}
	
	#ifdef DUP_CLIENTM_PTHREAD_DEBUG
	if (!simhashs)
	{
		fprintf(stdout, "[RESULT] -- simhash is not finded.\n");
	}
	else
	{
		fprintf(stdout, "[RESULT] -- simhash is finded (%d) as: \n", n_ret_count);
		PSimhashNode p_tmp_sh = simhashs;
		
		while (0 != p_tmp_sh)
		{
			fprintf(stdout, "%016llx\n", p_tmp_sh->hashcode);
			p_tmp_sh = p_tmp_sh->next;
		}
	}
	fprintf(stdout, "\n");
	#endif // DUP_CLIENTM_PTHREAD_DEBUG
	
	free(p_cta);
	return simhashs;
}

int dup_clientm_search_simhash(
	uint64_t hashcode, 
	uint64_t** simhashs, 
	uint64_t* size, 
	int f_insert
	)
{
	fprintf(stdout, "[INFO] -- You input a simhash is(hex): [%016llx]\n", hashcode);
	fprintf(stdout, "[INFO] -- You input a simhash is(bin): [%s]\n", binary_fmt(hashcode, g_tmp_64bit_string));
	
	pthread_t thread_ids[MAX_CHILD_PROCESS] = {0};
	int i = 0;
	for (i = 0; i < g_n_confnode_size; i ++)
	{
		#ifdef GET_HOST_PORT_CONF_DEBUG
		fprintf(stdout, "p_confnode[%d]->host = [%s]\n", i, p_confnode[i]->host);
		fprintf(stdout, "p_confnode[%d]->port = [%s]\n", i, p_confnode[i]->port);
		fprintf(stdout, "p_confnode[%d]->method = [%s]\n", i, p_confnode[i]->method);
		#endif // GET_HOST_PORT_CONF_DEBUG
		
		PClientmThreadArg p_cta = (PClientmThreadArg) malloc(sizeof(ClientmThreadArg));
		p_cta->pconfnode = p_confnode[i];
		p_cta->flag = ((1 == f_insert) ? (0x02) : (0x0));
		p_cta->code = hashcode;
		
		if (0 != pthread_create(&(thread_ids[i]), NULL, dup_clientm_search_simhash_thread, (void*)p_cta ) )
		{
			fprintf(stderr, "[ERROR] -- dup clientm create pthread in dup_clientm_search_simhash() failed!\n");
		}
		
	}
	
	// process the returned results
	PSimhashNode p_sh = NULL;
	PSimhashNode p_tmp_prev = NULL;
	PSimhashNode p_tmp_next = NULL;
	PSimhashNode p_tmp_dele = NULL;
	PSimhashNode p_tmp_ppre = NULL;
	uint64_t n_ret_count = 0;
	for (i = 0; i < g_n_confnode_size; i ++)
	{
		PSimhashNode p_tmp_sh = NULL;
		if (0 != pthread_join((thread_ids[i]), (void**)&p_tmp_sh) && (p_tmp_sh != NULL))
		{
			fprintf(stderr, "[ERROR] -- dup clientm pthread_join in dup_clientm_search_simhash() failed!\n");
		}
		else
		{
			// add thread result and deduplicate them
			p_tmp_next = p_tmp_sh;
			while (0 != p_tmp_next)
			{
				p_tmp_ppre = p_tmp_prev = p_sh;
				p_tmp_dele = NULL;
				while (0 != p_tmp_prev)
				{
					if (!((p_tmp_next->hashcode) ^ (p_tmp_prev->hashcode)))
					{
						p_tmp_dele = p_tmp_next;
						break;
					}
					p_tmp_ppre = p_tmp_prev;
					p_tmp_prev = p_tmp_prev->next;
				}
				if (NULL == p_tmp_dele)
				{
					n_ret_count ++;
					if (NULL != p_sh)
					{
						p_tmp_ppre->next = p_tmp_next;
						p_tmp_next = p_tmp_next->next;
						p_tmp_ppre->next->next = 0;
					}
					else
					{
						p_sh = p_tmp_next;
						p_tmp_next = p_tmp_next->next;
						p_sh->next = 0;
					}
				}
				else
				{
					p_tmp_next = p_tmp_next->next;
					free(p_tmp_dele);
				}
			}
		}
	}
	
	// tissue returns the result
	(*size) = n_ret_count;
	(*simhashs) = (uint64_t*) malloc(sizeof(uint64_t) * n_ret_count);
	for (i = 0; i < n_ret_count; i ++)
	{
		(*simhashs)[i] = p_sh->hashcode;
		p_tmp_prev = p_sh;
		p_sh = p_sh->next;
		free(p_tmp_prev);
	}
	
	return 0;
}

int dup_clientm_delete_simhash(
	uint64_t hashcode
	)
{
	fprintf(stdout, "[INFO] -- You want to delete a simhash is(hex): [%016llx]\n", hashcode);
	fprintf(stdout, "[INFO] -- You want to delete a simhash is(bin): [%s]\n", binary_fmt(hashcode, g_tmp_64bit_string));
	
	pthread_t thread_ids[MAX_CHILD_PROCESS] = {0};
	int i = 0;
	for (i = 0; i < g_n_confnode_size; i ++)
	{
		#ifdef GET_HOST_PORT_CONF_DEBUG
		fprintf(stdout, "p_confnode[%d]->host = [%s]\n", i, p_confnode[i]->host);
		fprintf(stdout, "p_confnode[%d]->port = [%s]\n", i, p_confnode[i]->port);
		fprintf(stdout, "p_confnode[%d]->method = [%s]\n", i, p_confnode[i]->method);
		#endif // GET_HOST_PORT_CONF_DEBUG
		
		PClientmThreadArg p_cta = (PClientmThreadArg) malloc(sizeof(ClientmThreadArg));
		p_cta->pconfnode = p_confnode[i];
		p_cta->flag = 0x01;
		p_cta->code = hashcode;
		
		if (0 != pthread_create(&(thread_ids[i]), NULL, dup_clientm_search_simhash_thread, (void*)p_cta ) )
		{
			fprintf(stderr, "[ERROR] -- dup clientm create pthread in dup_clientm_delete_simhash() failed!\n");
		}
	}
	
	// process the returned results
	for (i = 0; i < g_n_confnode_size; i ++)
	{
		if (0 != pthread_join((thread_ids[i]), NULL))
		{
			fprintf(stderr, "[ERROR] -- dup clientm pthread_join in dup_clientm_delete_simhash() failed!\n");
		}
	}
	
	return 0;
}

int dup_clientm_get_text_simhash(
	const char* text, 
	const int length, 
	uint64_t** simhashs, 
	uint64_t* size
	)
{
	return dup_clientm_search_simhash(charikar_hash64(text, length), 
		simhashs, 
		size, 
		1);
}

int dup_clientm_release(void)
{
	// free memory here.
	int i = 0;
	for (i = 0; i < g_n_confnode_size; i ++)
	{
		DUP_FREE(p_confnode[i]->host);
		DUP_FREE(p_confnode[i]->port);
		DUP_FREE(p_confnode[i]->method);
		DUP_FREE(p_confnode[i]);
	}
	DUP_FREE(p_confnode);
	
	return 0;
}

int main(int argc, char* argv[])
{
	dup_clientm_init();
	
	uint64_t hashcode = 0;
	fprintf(stdout, "[INPUT] -- Please input a simhash: ");
	if (EOF == scanf("%llx", &hashcode))
	{
		die(-1, "[ERROR] -- scanf a invalid hashcode!\n");
	}
	
	uint64_t* simhashs = NULL;
	uint64_t size = 0;
	// if (0 != dup_clientm_delete_simhash(hashcode))
	if (0 != dup_clientm_search_simhash(hashcode, &simhashs, &size, 1))
	{
		die(-1, "[ERROR] -- dup_clientm_search_simhash error!\n");
	}
	
	fprintf(stdout, "[INFO] -- result simhash count = %lld\n", size);
	int i = 0;
	for (i = 0; i < size; i ++)
	{
		fprintf(stdout, "[INFO] -- result simhash is(hex): [%016llx]\n", simhashs[i]);
	}
	free(simhashs);
	simhashs = NULL;
	
	dup_clientm_release();
	
	return 0;
}

/// #define DUP_CLIENTM_MAIN_TEST
#ifdef DUP_CLIENTM_MAIN_TEST

int main(int argc, char* argv[])
{
	// read configure file
	char *data = NULL;
	long file_length = 0;
	get_file(DUP_SYSCONFIG_DIR"/dup.client.conf", &data, &file_length);
	
	// analysis configure setting
	PDupConfNode* p_confnode = NULL;
	int g_n_confnode_size = 0;
	if (get_host_port_conf(data, &p_confnode, &g_n_confnode_size))
	{
		fprintf(stderr, "[ERROR] -- get_host_port_conf error.\n");
		return -1;
	}
	
	/// !! commond test here...
	uint64_t hashcode = 0;
	// input a hash code
	/// #ifdef DUP_TEST_HASHCODE
	if (argc > 1 && 0 == strcmp(argv[1], "file"))
	{
		char filepath[1024] = {0};
		char* text = NULL;
		long text_length = 0;
		fprintf(stdout, "[INPUT] -- Please input a filepath: ");
		if (EOF != scanf("%s", filepath))
		{
			get_file(filepath, &text, &text_length);
			hashcode = (uint64_t)(charikar_hash64(text, text_length));
			fprintf(stdout, "[INFO] -- You input a text is: [%s]\n", text);
		}
	}
	else
	{
		fprintf(stdout, "[INPUT] -- Please input a simhash: ");
		if (EOF == scanf("%llx", &hashcode))
		{
			die(-1, "[ERROR] -- scanf a invalid hashcode!\n");
		}
	}
	/// #else // DUP_TEST_HASHCODE
	/// #endif // DUP_TEST_HASHCODE
	fprintf(stdout, "[INFO] -- You input a simhash is(hex): [%016llx]\n", hashcode);
	fprintf(stdout, "[INFO] -- You input a simhash is(bin): [%s]\n", binary_fmt(hashcode, g_tmp_64bit_string));
	
	int i = 0;
	fprintf(stdout, "[INFO] -- start client thread count: %d\n", g_n_confnode_size);
	SET_RUN_TIME
	int _x = 0; for (_x = 0; _x < 100; _x ++) { fprintf(stdout, "[DEBUG] -- _x = %d\n", _x);
	for (i = 0; i < g_n_confnode_size; i ++)
	{
		#ifdef GET_HOST_PORT_CONF_DEBUG
		fprintf(stdout, "p_confnode[%d]->host = [%s]\n", i, p_confnode[i]->host);
		fprintf(stdout, "p_confnode[%d]->port = [%s]\n", i, p_confnode[i]->port);
		fprintf(stdout, "p_confnode[%d]->method = [%s]\n", i, p_confnode[i]->method);
		#endif // GET_HOST_PORT_CONF_DEBUG
		
		PSimhashNode simhashs = 0;
		int n_ret_count = 0;
		
		if (dup_client(p_confnode[i]->host, 
			atoi(p_confnode[i]->port), 
			get_simhash_split_method(p_confnode[i]->method), 
			hashcode, 
			&simhashs, 
			&n_ret_count, 
			1, 
			0))
		{
			fprintf(stderr, "[ERROR] -- dup client invoke connect failed!\n");
		}
		
		if (!simhashs)
		{
			fprintf(stdout, "[RESULT] -- simhash is not finded.\n");
		}
		else
		{
			fprintf(stdout, "[RESULT] -- simhash is finded (%d) as: \n", n_ret_count);
			PSimhashNode p_tmp_sh = simhashs;
			
			while (0 != p_tmp_sh)
			{
				fprintf(stdout, "%016llx\n", p_tmp_sh->hashcode);
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
		fprintf(stdout, "\n");
	}
	}
	GET_RUN_TIME
	
	// free memory here.
	for (i = 0; i < g_n_confnode_size; i ++)
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

#endif // DUP_CLIENTM_MAIN_TEST

