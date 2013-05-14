
#include "dup.h"
#include "common.h"

/// #define DUP_DEBUG

PSimhashIndex create_simhash_index(int bit, f_simhash_split_method simhash_split)
{
	fprintf(stdout, "[INFO] -- begin to create simhash index table...\n");
	
	PSimhashIndex shindex = (PSimhashIndex) malloc(sizeof(SimhashIndex));
	if (0 == shindex)
	{
		die(-1, "[ERROR] -- create_simhash_index failed as no more memory to apply for.\n");
	}
	shindex->bit = bit;
	shindex->simhash_split = simhash_split;
	shindex->count = 0;
	size_t size = pow(2, bit);
	shindex->table = (PSimhashNode*)malloc(size * sizeof(PSimhashNode));
	if (0 == shindex)
	{
		die(-1, "[ERROR] -- create_simhash_index failed as no more memory to apply for.\n");
	}
	memset(shindex->table, 0, size * sizeof(PSimhashNode));
	
	fprintf(stdout, "[INFO] -- create simhash index table completed.\n");
	
	return shindex;
}

int release_simhash_index(PSimhashIndex shindex)
{
	fprintf(stdout, "[INFO] -- begin to release simhash index table...\n");
	
	if (0 != shindex)
	{
		if (0 != shindex->table)
		{
			// traverse table to release nodes
			int i = 0;
			size_t size = pow(2, shindex->bit);
			PSimhashNode p_tmp_sh = 0;
			for (i = 0; i < size; i ++)
			{
				while (0 != shindex->table[i])
				{
					p_tmp_sh = shindex->table[i];
					shindex->table[i] = p_tmp_sh->next;
					free(p_tmp_sh);
				}
			}
			free(shindex->table);
			shindex->table = 0;
		}
		free(shindex);
		shindex = 0;
	}
	
	fprintf(stdout, "[INFO] -- release simhash index table completed.\n");
	
	return 0;
}

int read_from_file(
		PSimhashIndex shindex, 
		const char* file_path
	)
{
	// file data structure: first simhash is store the count of total hash count.
	// then all data structure is store the simhash code.
	FILE* fp = 0;
	if((fp = fopen(file_path, "rb")) == 0)
	{
		die(-1, "[ERROR] -- dup simhash index table read from file (%s) failed\n", 
			file_path);
	}
	
	if (0 != shindex)
	{
		if (0 != shindex->table)
		{
			fprintf(stdout, "[INFO] -- now is read simhash index table from file...\n");
			
			uint64_t n_count = 0;
			if (1 != fread(&(n_count), sizeof(uint64_t), 1, fp))
			{
				die(-1, "[ERROR] -- dup simhash index table fread() count failed\n");
			}
			
			int i = 0;
			uint64_t tmp_hashcode = 0;
			for (i = 0; i < n_count; i ++)
			{
				if (1 != fread(&(tmp_hashcode), sizeof(uint64_t), 1, fp))
				{
					die(-1, "[ERROR] -- dup simhash index table fread() hashcode failed\n");
				}
				if (insert_simhash(shindex, tmp_hashcode))
				{
					die(-1, "[ERROR] -- dup simhash index table insert in read file failed\n");
				}
			}
			
			fprintf(stdout, "[INFO] -- read %lld simhash to file completed.\n", n_count);
		}
	}
	
	if (fclose(fp))
	{
		die(-1, "[ERROR] -- dup simhash index table read file close failed\n");
	}
	
	fprintf(stdout, "[INFO] -- now there are %lld simhash in table.\n", shindex->count);
	
	return 0;
}

int write_to_file(
		PSimhashIndex shindex, 
		const char* file_path, 
		int is_lock
	)
{
	// file data structure: first simhash is store the count of total hash count.
	// then all data structure is store the simhash code.
	FILE* fp = 0;
	if((fp = fopen(file_path, "wb")) == 0)
	{
		die(-1, "[ERROR] -- dup simhash index table write to file failed\n");
	}
	
	// lock file
	if (0 != is_lock)
	{
		if ((flock(fileno(fp), LOCK_EX | LOCK_NB))< 0)
		{
			fprintf(stderr, "[LOG] -- The hash table file have been locked.\n");
			return -1;
		}
		fprintf(stderr, "[LOG] -- The hash table file locked OK.\n");
	}
	
	if (0 != shindex)
	{
		if (0 != shindex->table)
		{
			if (1 != fwrite(&(shindex->count), sizeof(uint64_t), 1, fp))
			{
				die(-1, "[ERROR] -- dup simhash index table fwrite() count failed\n");
			}
			
			fprintf(stdout, "[INFO] -- now is writing simhash index table...\n");
			
			// traverse table to release nodes
			int i = 0;
			size_t size = pow(2, shindex->bit);
			for (i = 0; i < size; i ++)
			{
				PSimhashNode p_tmp_sh = shindex->table[i];
				while (0 != p_tmp_sh)
				{
					if (1 != fwrite(&(p_tmp_sh->hashcode), sizeof(uint64_t), 1, fp))
					{
						die(-1, "[ERROR] -- dup simhash index table fwrite() hashcode failed\n");
					}
					
					p_tmp_sh = p_tmp_sh->next;
				}
			}
		}
	}
	
	if (fclose(fp))
	{
		die(-1, "[ERROR] -- dup simhash index table write file close failed\n");
	}
	
	// unlock file
	if (0 != is_lock)
	{
		if ((flock(fileno(fp), LOCK_UN))<0)
		{
			fprintf(stderr, "[LOG] -- The hash table file unlock error.\n");
			return -1;
		}
		fprintf(stderr, "[LOG] -- The hash table file unlock OK.\n");
	}
	
	fprintf(stdout, "[INFO] -- write %lld simhash to file completed.\n", shindex->count);
	
	return 0;
}

int insert_simhash(PSimhashIndex shindex, uint64_t hashcode)
{
	if (0 != shindex)
	{
		if (0 != shindex->table)
		{
			size_t _i = shindex->simhash_split(hashcode);
			// invalid hashcode split here.
			if (!(hashcode ^ 0xffffffffffffffffULL))
			{
				fprintf(stdout, "[WARNNING] -- insert invalid hashcode(hex): [%016llx]\n", 
					hashcode);
			}
			// avoid duplication simhash code.
			PSimhashNode p_tmp_sh = shindex->table[ _i ];
			while (0 != p_tmp_sh)
			{
				if (!(hashcode ^ (p_tmp_sh->hashcode)))
				{
					return 0;
				}
				p_tmp_sh = p_tmp_sh->next;
			}
			// insert new simhash note in the header.
			PSimhashNode p_sh = (PSimhashNode) malloc(sizeof(SimhashNode));
			p_sh->hashcode = hashcode;
			p_sh->next = shindex->table[ _i ];
			shindex->table[ _i ] = p_sh;
			shindex->count ++;
			
			return 0;
		}
	}
	
	return -1;
}

int search_and_insert_simhash(PSimhashIndex shindex, 
	uint64_t hashcode, 
	PSimhashNode* p_phashcode, 
	int* p_count, 
	int f_insert)
{
	(*p_count) = 0;
	(*p_phashcode) = 0;
	if (0 != shindex)
	{
		if (0 != shindex->table)
		{
			int f_duplication = false;
			size_t _i = shindex->simhash_split(hashcode);
			PSimhashNode p_tmp_sh = shindex->table[ _i ];
			while (0 != p_tmp_sh)
			{
				int dist = hamming_dist(hashcode, p_tmp_sh->hashcode);
				if (dist <= SIMILAR_HAMMING_DIST)
				{
					PSimhashNode p_tmp_node_sh = (PSimhashNode) malloc(sizeof(SimhashNode));
					p_tmp_node_sh->hashcode = p_tmp_sh->hashcode;
					
					#ifdef DUP_DEBUG
					fprintf(stdout, "[DEBUG] ------> [%016llx]\n", 
						p_tmp_sh->hashcode);
					#endif // DUP_DEBUG
					
					p_tmp_node_sh->next = (*p_phashcode);
					(*p_phashcode) = p_tmp_node_sh;
					(*p_count) ++;
					
					// recode duplication
					if (0 == dist) f_duplication = true;
				}
				p_tmp_sh = p_tmp_sh->next;
			}
			
			// insert when not any duplication accur.
			if ((!f_duplication) && (0 != f_insert))
			{
				PSimhashNode p_sh = (PSimhashNode) malloc(sizeof(SimhashNode));
				p_sh->hashcode = hashcode;
				p_sh->next = shindex->table[ _i ];
				shindex->table[ _i ] = p_sh;
				shindex->count ++;
			}
			
			return 0;
		}
	}
	
	return -1;
}

int delete_simhash(PSimhashIndex shindex, uint64_t hashcode)
{
	if (0 != shindex)
	{
		if (0 != shindex->table)
		{
			size_t _i = shindex->simhash_split(hashcode);
			
			// avoid duplication simhash code.
			PSimhashNode p_tmp_sh = shindex->table[ _i ];
			PSimhashNode p_tmp_prev = NULL;
			PSimhashNode p_tmp_dele = NULL;
			while (0 != p_tmp_sh)
			{
				if (!(hashcode ^ (p_tmp_sh->hashcode)))
				{
					p_tmp_dele = p_tmp_sh;
					
					if (p_tmp_prev)
					{
						p_tmp_prev->next = p_tmp_sh->next;
					}
					else
					{
						shindex->table[ _i ] = p_tmp_sh->next;
					}
					
					fprintf(stdout, "[INFO] delete hashcode: [%016llx] completed.\n", 
						p_tmp_dele->hashcode);
					
					free(p_tmp_dele);
					shindex->count --;
					return 0;
				}
				p_tmp_prev = p_tmp_sh;
				p_tmp_sh = p_tmp_sh->next;
			}
		}
	}
	
	fprintf(stderr, "[WARNNING] --  Could not find hashcode [%016llx] to delete.\n", 
		hashcode);
	
	return -1;
}

int insert_text_simhash(
	PSimhashIndex shindex, 
	char* text, 
	long length)
{
	return insert_simhash(
		shindex, 
		charikar_hash64(text, length));
}

int search_and_insert_text_simhash(
	PSimhashIndex shindex, 
	char* text, 
	long length, 
	PSimhashNode* p_phashcode, 
	int* p_count)
{
	#ifdef DUP_DEBUG
	fprintf(stdout, "[DEBUG] -- charikar_hash64([%s], [%d]) = [%016llx]\n", 
		text, length, (uint64_t)(charikar_hash64(text, length)));
	#endif // DUP_DEBUG
	
	return search_and_insert_simhash(
		shindex, 
		charikar_hash64(text, length), 
		p_phashcode, 
		p_count, 
		1);
}

int insert_text_simhashw(
	PSimhashIndex shindex, 
	char* text, 
	long length, 
	int stoken_size)
{
	return insert_simhash(
		shindex, 
		charikar_hash64_wide(text, length, stoken_size));
}

int search_and_insert_text_simhashw(
	PSimhashIndex shindex, 
	char* text, 
	long length, 
	int stoken_size, 
	PSimhashNode* p_phashcode, 
	int* p_count)
{
	return search_and_insert_simhash(
		shindex, 
		charikar_hash64_wide(text, length, stoken_size), 
		p_phashcode, 
		p_count, 
		1);
}


/// #define DUP_MAIN_TEST
#ifdef DUP_MAIN_TEST

#include "dup.h"
#include "util.h"
#include "split.h"
#include "runtime.h"
#include "common.h"
#include <time.h>
#include <stdlib.h>

#define TOTAL_SIMHASH_COUNT		30000000

/// #define DUP_TEST_INFO

int main(int argc, char* argv[])
{
	PSimhashIndex index_12_13_1 = 
		create_simhash_index((12 + 13), simhash_split_12_13_1);
	
	// Use Random to insert total hashcode.
	srand((unsigned)time(NULL));
	int i = 0;
	for (i = 0; i < TOTAL_SIMHASH_COUNT; i ++)
	{
		uint64_t hashcode = rand();
		hashcode <<= 32;
		hashcode |= rand();
		insert_simhash(index_12_13_1, hashcode);
	}
	
	SET_RUN_TIME
	// input hashcode to find similiar simhash.
	/// if (EOF != scanf("%llx", &input_hashcode))
	for (i = 0; i < 1000000; i ++)
	{
		uint64_t input_hashcode = 0;
		PSimhashNode simhashs = 0;
		int n_ret_count = 0;
		
		#ifdef DUP_TEST_INFO
		fprintf(stdout, "[INPUT] -- Please input a simhash: ");
		#endif // DUP_TEST_INFO
		
		input_hashcode = rand();
		input_hashcode <<= 32;
		input_hashcode |= rand();
		
		#ifdef DUP_TEST_INFO
		fprintf(stdout, "[INFO] -- You input a simhash is(hex): [%016llx]\n", input_hashcode);
		fprintf(stdout, "[INFO] -- You input a simhash is(bin): [%s]\n", binary_fmt(input_hashcode, g_tmp_64bit_string));
		#endif // DUP_TEST_INFO
		
		if (search_and_insert_simhash(index_12_13_1, input_hashcode, &simhashs, &n_ret_count, 1))
		{
			die(-1, "[ERROR] -- search_and_insert_simhash failed.\n");
		}
		
		if (!simhashs)
		{
			#ifdef DUP_TEST_INFO
			fprintf(stdout, "[RESULT] -- simhash is not finded.\n");
			#endif // DUP_TEST_INFO
		}
		else
		{
			#ifdef DUP_TEST_INFO
			fprintf(stdout, "[RESULT] -- simhash is finded (%d) as: \n", n_ret_count);
			#endif // DUP_TEST_INFO
			PSimhashNode p_tmp_sh = simhashs;
			
			while (0 != p_tmp_sh)
			{
				#ifdef DUP_TEST_INFO
				fprintf(stdout, "%016llx\n", p_tmp_sh->hashcode);
				#endif // DUP_TEST_INFO
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
		#ifdef DUP_TEST_INFO
		fprintf(stdout, "\n");
		#endif // DUP_TEST_INFO
		
	}
	
	GET_RUN_TIME
	
	if (release_simhash_index(index_12_13_1))
	{
		die(-1, "[ERROR] -- release_simhash_index failed.\n");
	}
	
	return 0;
}

#endif // DUP_MAIN_TEST

/// #define DUP_TEXT_MAIN_TEST
#ifdef DUP_TEXT_MAIN_TEST

#include "dup.h"
#include "util.h"
#include "split.h"
#include "runtime.h"
#include "common.h"
#include <time.h>
#include <stdlib.h>

#define TOTAL_SIMHASH_COUNT		30000000

#define RANDOM_GENERATE 0
#define DUP_TEST_INFO

int main(int argc, char* argv[])
{
	PSimhashIndex index_12_13_1 = 
		create_simhash_index((12 + 13), simhash_split_12_13_1);
	
	// Use Random to insert total hashcode.
	int i = 0;
	
	#if RANDOM_GENERATE
	fprintf(stdout, "[INFO] -- begin to generate random simhash codes...\n");
	
	srand((unsigned)time(NULL));
	for (i = 0; i < TOTAL_SIMHASH_COUNT; i ++)
	{
		uint64_t hashcode = rand();
		hashcode <<= 32;
		hashcode |= rand();
		insert_simhash(index_12_13_1, hashcode);
	}
	
	fprintf(stdout, "[INFO] -- insert generate random simhash codes completed.\n");
	
	// Write simhash index table to file.
	if (write_to_file(index_12_13_1, "index_12_13_1.dup", 0))
	{
		die(-1, "[ERROR] -- write_to_file simhash table failed.\n");
	}
	
	#else
	
	// Write simhash index table to file.
	if (read_from_file(index_12_13_1, "index_12_13_1.dup"))
	{
		die(-1, "[ERROR] -- read_from_file simhash table failed.\n");
	}
	
	#endif // RANDOM_GENERATE
	
	SET_RUN_TIME
	// input hashcode to find similiar simhash.
	/// if (EOF != scanf("%llx", &input_hashcode))
	for (i = 0; i < 3; i ++)
	{
		char text[1024] = {0};
		uint64_t input_hashcode = 0;
		PSimhashNode simhashs = 0;
		int n_ret_count = 0;
		
		#ifdef DUP_TEST_INFO
		fprintf(stdout, "[INPUT] -- Please input a simhash: ");
		#endif // DUP_TEST_INFO
		
		if (EOF != scanf("%s", text))
		input_hashcode = (uint64_t)(charikar_hash64(text, strlen(text)));
		
		#ifdef DUP_TEST_INFO
		fprintf(stdout, "[INFO] -- You input a text is: [%s]\n", text);
		fprintf(stdout, "[INFO] -- You input a simhash is(hex): [%016llx]\n", input_hashcode);
		fprintf(stdout, "[INFO] -- You input a simhash is(bin): [%s]\n", binary_fmt(input_hashcode, g_tmp_64bit_string));
		#endif // DUP_TEST_INFO
		
		if (search_and_insert_text_simhash(index_12_13_1, text, strlen(text), &simhashs, &n_ret_count))
		{
			die(-1, "[ERROR] -- search_and_insert_text_simhash failed.\n");
		}
		
		if (!simhashs)
		{
			#ifdef DUP_TEST_INFO
			fprintf(stdout, "[RESULT] -- simhash is not finded.\n");
			#endif // DUP_TEST_INFO
		}
		else
		{
			#ifdef DUP_TEST_INFO
			fprintf(stdout, "[RESULT] -- simhash is finded (%d) as: \n", n_ret_count);
			#endif // DUP_TEST_INFO
			PSimhashNode p_tmp_sh = simhashs;
			
			while (0 != p_tmp_sh)
			{
				#ifdef DUP_TEST_INFO
				fprintf(stdout, "%016llx\n", p_tmp_sh->hashcode);
				#endif // DUP_TEST_INFO
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
		#ifdef DUP_TEST_INFO
		fprintf(stdout, "\n");
		#endif // DUP_TEST_INFO
		
	}
	
	GET_RUN_TIME
	
	if (release_simhash_index(index_12_13_1))
	{
		die(-1, "[ERROR] -- release_simhash_index failed.\n");
	}
	
	return 0;
}

#endif // DUP_TEXT_MAIN_TEST

