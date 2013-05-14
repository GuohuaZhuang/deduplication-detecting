#include <stdio.h>
#include <dup/clientm.h>

// complile: gcc [your code file] -ldup
int main(int argc, char* argv[])
{
    dup_clientm_init();

    uint64_t hashcode = 0;
///    fprintf(stdout, "[INPUT] -- Please input a simhash: ");
///    if (EOF == scanf("%llx", &hashcode))
///    {
///        die(-1, "[ERROR] -- scanf a invalid hashcode!\n");
///    }
	
	if ( dup_clientm_get_text_hashcode(argv[1], strlen(argv[1]), &hashcode) )
	{
		die(-1, "[ERROR] -- get text's hashcode failed.\n");
	}
    fprintf(stdout, "[INFO] -- result simhash is(hex): [%016llx]\n", hashcode);

///    uint64_t* simhashs = NULL;
///    uint64_t size = 0;
///    if (0 != dup_clientm_search_simhash(hashcode, &simhashs, &size, 1))
///    {
///        die(-1, "[ERROR] -- dup_clientm_search_simhash error!\n");
///    }
///
///    fprintf(stdout, "[INFO] -- result simhash count = %lld\n", size);
///    int i = 0;
///    for (i = 0; i < size; i ++)
///    {
///        fprintf(stdout, "[INFO] -- result simhash is(hex): [%016llx]\n", simhashs[i]);
///    }
///    free(simhashs);
///    simhashs = NULL;

    dup_clientm_release();

    return 0;
}

