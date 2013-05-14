
#include "conf.h"
#include "define.h"
#include <string.h>

//. #define GET_HOST_PORT_CONF_DEBUG

int get_host_port_conf(const char* s, PDupConfNode** p_confnode, int* n_size)
{
	// string, delim_line, delim_elem
	char* str = NULL;
	char* str_tmp = NULL;
	DUP_C_STRCPY(str, s);
	DUP_C_STRCPY(str_tmp, s);
	const char* delim_line = "\n";
	const char* delim_elem = ":";
	
	char *str1, *str2, *token, *subtoken;
	char *saveptr1, *saveptr2;
	int j = 0;
	(*n_size) = 0;
	
	// calculate the line count
	for (j = 0, str1 = str_tmp; ; j++, str1 = NULL)
	{
		token = strtok_r(str1, delim_line, &saveptr1);
		if (token == NULL) break;
		(*n_size) ++;
		
		#ifdef GET_HOST_PORT_CONF_DEBUG
		fprintf(stdout, "n_size ---> %d\n", (*n_size));
		fprintf(stdout, "saveptr1 = [%s]\n", saveptr1);
		#endif // GET_HOST_PORT_CONF_DEBUG
	}
	
	// apply for memory
	(*p_confnode) = (PDupConfNode*) malloc(sizeof(PDupConfNode) * (*n_size));
	memset((*p_confnode), 0, sizeof(PDupConfNode*) * (*n_size));
	
	// split to extract info
	for (j = 0, str1 = str; ; j++, str1 = NULL)
	{
		token = strtok_r(str1, delim_line, &saveptr1);
		if (token == NULL) break;
		
		(*p_confnode)[j] = (PDupConfNode) malloc(sizeof(DupConfNode));
		
		str2 = token;
		{
			subtoken = strtok_r(str2, delim_elem, &saveptr2);
			DUP_C_STRCPY((*p_confnode)[j]->host, subtoken);
			subtoken = strtok_r(NULL, delim_elem, &saveptr2);
			DUP_C_STRCPY((*p_confnode)[j]->port, subtoken);
			subtoken = strtok_r(NULL, delim_elem, &saveptr2);
			DUP_C_STRCPY((*p_confnode)[j]->method, subtoken);
		}
	}
	
	DUP_FREE(str);
	DUP_FREE(str_tmp);
	
	return 0;
}

f_simhash_split_method get_simhash_split_method(const char* s)
{
	if (0 == strcmp(s, "simhash_split_12_13_1"))
	{
		return simhash_split_12_13_1;
	}
	else if (0 == strcmp(s, "simhash_split_12_13_2"))
	{
		return simhash_split_12_13_2;
	}
	else if (0 == strcmp(s, "simhash_split_12_13_3"))
	{
		return simhash_split_12_13_3;
	}
	else if (0 == strcmp(s, "simhash_split_12_13_4"))
	{
		return simhash_split_12_13_4;
	}
	else if (0 == strcmp(s, "simhash_split_13_13_1"))
	{
		return simhash_split_13_13_1;
	}
	else if (0 == strcmp(s, "simhash_split_13_13_2"))
	{
		return simhash_split_13_13_2;
	}
	else if (0 == strcmp(s, "simhash_split_13_13_3"))
	{
		return simhash_split_13_13_3;
	}
	else if (0 == strcmp(s, "simhash_split_13_13_4"))
	{
		return simhash_split_13_13_4;
	}
	else if (0 == strcmp(s, "simhash_split_13_13_5"))
	{
		return simhash_split_13_13_5;
	}
	else if (0 == strcmp(s, "simhash_split_13_13_6"))
	{
		return simhash_split_13_13_6;
	}
	
	return 0;
}

int get_simhash_split_bit(const char* s)
{
	if (0 == strcmp(s, "simhash_split_12_13_1"))
	{
		return (12 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_12_13_2"))
	{
		return (12 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_12_13_3"))
	{
		return (12 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_12_13_4"))
	{
		return (12 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_13_13_1"))
	{
		return (13 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_13_13_2"))
	{
		return (13 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_13_13_3"))
	{
		return (13 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_13_13_4"))
	{
		return (13 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_13_13_5"))
	{
		return (13 + 13);
	}
	else if (0 == strcmp(s, "simhash_split_13_13_6"))
	{
		return (13 + 13);
	}
	
	return 0;
}

char g_simhash_table_filepath[10][1024] = {
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup", 
	DUP_DATA_DIR"/deduplication/index_table.dup" 
	};

char* get_simhash_table_filepath(const char* s)
{
	if (0 == strcmp(s, "simhash_split_12_13_1"))
	{
		return g_simhash_table_filepath[0];
	}
	else if (0 == strcmp(s, "simhash_split_12_13_2"))
	{
		return g_simhash_table_filepath[1];
	}
	else if (0 == strcmp(s, "simhash_split_12_13_3"))
	{
		return g_simhash_table_filepath[2];
	}
	else if (0 == strcmp(s, "simhash_split_12_13_4"))
	{
		return g_simhash_table_filepath[3];
	}
	else if (0 == strcmp(s, "simhash_split_13_13_1"))
	{
		return g_simhash_table_filepath[4];
	}
	else if (0 == strcmp(s, "simhash_split_13_13_2"))
	{
		return g_simhash_table_filepath[5];
	}
	else if (0 == strcmp(s, "simhash_split_13_13_3"))
	{
		return g_simhash_table_filepath[6];
	}
	else if (0 == strcmp(s, "simhash_split_13_13_4"))
	{
		return g_simhash_table_filepath[7];
	}
	else if (0 == strcmp(s, "simhash_split_13_13_5"))
	{
		return g_simhash_table_filepath[8];
	}
	else if (0 == strcmp(s, "simhash_split_13_13_6"))
	{
		return g_simhash_table_filepath[9];
	}
	
	return 0;
}

/// #define CONF_CONFIGURE_MAIN_TEST
#ifdef CONF_CONFIGURE_MAIN_TEST

int main(int argc, char* argv[])
{
	if (argc < 2) return -1;
	
	// read configure file
	char *data = NULL;
	long file_length = 0;
	get_file(DUP_SYSCONFIG_DIR"/dup.client.conf", &data, &file_length);
	
	// analysis configure setting
	PDupConfNode* p_confnode = NULL;
	int n_size = 0;
	if (get_host_port_conf(data, &p_confnode, &n_size))
	{
		fprintf(stderr, "[ERROR] -- get_host_port_conf error.\n");
		return -1;
	}
	
	int i = 0;
	
	fprintf(stdout, "n_size = %d\n", n_size);
	for (i = 0; i < n_size; i ++)
	{
		fprintf(stdout, "p_confnode[%d]->host = [%s]\n", i, p_confnode[i]->host);
		fprintf(stdout, "p_confnode[%d]->port = [%s]\n", i, p_confnode[i]->port);
		fprintf(stdout, "p_confnode[%d]->method = [%s]\n", i, p_confnode[i]->method);
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
	
	return 0;
}

#endif // CONF_CONFIGURE_MAIN_TEST


