
#include "util.h"

/** Prints a message to stderr and exits.
 */
void die(int ex, const char *fmt, ...)
{
	va_list ap;
	
	fflush( stdout);
	
	if (fmt && *fmt)
	{
	        va_start( ap, fmt);
		vfprintf( stderr, fmt, ap);
		va_end( ap);
		
		fprintf( stderr, "\n");
	}
	
	exit( ex);
}

/** Reads the file specified.
 */
void get_file(char *file_name, char **data, long *file_length)
{
	/* Open input file */
	FILE *input = fopen( file_name, "r");
	if (!input)
		die( 1, "[ERROR] -- Can't open file \"%s\": %m.", file_name);

	/* Get file length */
	fseek( input, 0, SEEK_END);
	*file_length = ftell( input);
	fseek( input, 0, SEEK_SET);

	/* Get the buffer */
	*data = (char *)malloc( (*file_length) + 1);
	(*data)[(*file_length)] = '\0';
	if (!*data)
		die( 2, "[ERROR] -- Out of memoru.\n");

	/* Read the file */
	size_t readed = fread( *data, 1, *file_length, input);
	if (readed != *file_length)
		/* EOF cannot happen */
		die( 3, "[ERROR] -- Error reading file \"%s\": %m.", file_name);

	/* Close the file */
	if (fclose( input) == EOF)
		die( 4, "[ERROR] -- Can't close file \"%s\": %m.", file_name);
}

char g_tmp_64bit_string[FMT_BUF_SIZE];

char* binary_fmt(uintmax_t x, char buf[FMT_BUF_SIZE])
{
    char *s = buf + FMT_BUF_SIZE;
    *--s = 0;
    if (!x) *--s = '0';
    for(; x || s > buf; x/=2)
    {
    	*--s = '0' + x%2;
    }
    return s;
}

