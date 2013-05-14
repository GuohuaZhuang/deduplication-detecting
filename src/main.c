
#include "dup.h"
#include "util.h"
#include "split.h"
#include "runtime.h"
#include "common.h"
#include <time.h>
#include <stdlib.h>

#define TOTAL_SIMHASH_COUNT		30000000

#define RANDOM_GENERATE 1
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
		
		if (delete_simhash(index_12_13_1, input_hashcode))
		{
			fprintf(stderr, "[ERROR] -- delete_simhash failed.\n");
		}
		if (search_and_insert_text_simhash(index_12_13_1, text, strlen(text), &simhashs, &n_ret_count))
		{
			die(-1, "[ERROR] -- search_and_insert_text_simhash failed.\n");
		}
		if (delete_simhash(index_12_13_1, input_hashcode))
		{
			fprintf(stderr, "[ERROR] -- delete_simhash failed.\n");
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

