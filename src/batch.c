
#include "batch.h"

#define BATCH_DEBUG_INFO

PSimhashIndex dup_batch_init(
	f_simhash_split_method simhash_split, 
	int bit)
{
	PSimhashIndex index_table = 
		create_simhash_index(bit, simhash_split);
	return index_table;
}

int dup_batch_insert_text_simhash(
	PSimhashIndex index_table, 
	const char* text, 
	const int length)
{
	uint64_t hashcode = (uint64_t)(charikar_hash64(text, length));
	
	#ifdef BATCH_DEBUG_INFO
	fprintf(stdout, "[INFO] -- Your text convert to a simhash is(hex): [%016llx]\n", hashcode);
	#endif // BATCH_DEBUG_INFO
	
	if (insert_simhash(index_table, hashcode))
	{
		fprintf(stderr, "[ERROR] -- dup batch insert_simhash() failed.\n");
		return -1;
	}
	
	return 0;
}

int dup_batch_release(
	PSimhashIndex index_table, 
	const char* index_file_path)
{
	// Write simhash index table to file.
	if (write_to_file(index_table, index_file_path, 0))
	{
		die(-1, "[ERROR] -- write_to_file simhash table failed.\n");
	}
	if (release_simhash_index(index_table))
	{
		die(-1, "[ERROR] -- release_simhash_index failed.\n");
	}
	
	return 0;
}

#define BATCH_MAIN_TEST
#ifdef BATCH_MAIN_TEST
int main(int argc, char* argv[])
{
	PSimhashIndex index_table = dup_batch_init(simhash_split_12_13_1, (12 + 13));
	
	if (0 == index_table)
	{
		die(-1, "[ERROR] -- dup_batch_init() error.\n");
	}
	
	const char* text_content = "this is text content.";
	if (dup_batch_insert_text_simhash(
		index_table, 
		text_content, 
		strlen(text_content)) )
	{
		die(-1, "[ERROR] -- dup_batch_insert_text_simhash() error.\n");
	}
	
	if (dup_batch_release(index_table, "./index_table.dup"))
	{
		die(-1, "[ERROR] -- dup_batch_release() error.\n");
	}
	
	return 0;
}
#endif // BATCH_MAIN_TEST

