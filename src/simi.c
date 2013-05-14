/*
 * Implementations of the M. Charikar's document similarity hash.
 *
 * Public domain
 * 2007 Viliam Holub
 */

#include <ctype.h>
#include <stdint.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>

#include "hashmap.h"
#include "lookup3.h"

/* Debug mode */
#define DEBUG 0

#if DEBUG
#	include <stdio.h>
#	define dprintf(...) fprintf( stderr, __VA_ARGS__)
#else
#	define dprintf(...)
#endif


/** Filters unimportant characters.
 * Now important characters are alphanumeric and those with the most
 * significant bit set (utf-8 friendly).
 */
static bool
meaningful_char( char c)
{
	return isalnum( c) || (c&0x80);
}


#if DEBUG
/** Converts a pair of address and length to a C-string.
 * Returns an internal buffer!
 */
static const char *
string( const char *string_start, size_t string_length)
{
	static char sbuf[ 1024];
	int c;
	int stop = string_length < 1023 ? string_length : 1023;
	for (c=0; c<stop; c++)
		sbuf[ c] = string_start[ c];
	sbuf[ c] = '\0';

	return sbuf;
}
#endif

/** Returns next token.
 * string	current data pointer
 * string_end	pointer one character behind the end of data
 * token_start	output hash value of the token
 * token_length
 */
static bool
get_next_token( const char **string, const char *string_end, const char **token_start, size_t *token_length)
{
	const char *start = *string;

	/* Skip white space */
	while (start != string_end && !meaningful_char( *start))
		start++;

	/* At the end of the string? */
	if (start == string_end) {
		*string = start;
		dprintf( " token: end\n");
		return false;
	}
	
	/* Read the string */
	const char *s = start+1;
	while (s != string_end && meaningful_char( *s)) {
		s++;
	}

	*string = s;
	*token_start = start;
	*token_length = s-start;
	return true;
}


static int GetFirstAtomsLengthUTF8(const char* str, const char *string_end)
{
	if(0 == (int)str || string_end <= str)
	{
		return 0;
	}
	/** One encoded. ASCII charactor. */
	if(!(str[0] & 0x80))
	{
		int offset = 0;
		while(string_end > str+offset && str[offset] && !(str[offset] & 0x80)/* && isalnum(str[offset])*/
			&& (!isspace(str[offset])))
		{
			offset++;
		}
		return (offset > 1) ? offset : 1;
	}
	/** Two encoded. */
	else if(!((str[0] & 0xE0) ^ 0xC0) && 
			!((str[1] & 0xC0) ^ 0x80))
	{
		return 2;
	}
	/** Three encoded. */
	else if(!((str[0] & 0xF0) ^ 0xE0) && 
			!((str[1] & 0xC0) ^ 0x80) && 
			!((str[2] & 0xC0) ^ 0x80))
	{
		return 3;
	}
	/** Four encoded. */
	else if(!((str[0] & 0xF8) ^ 0xF0) && 
			!((str[1] & 0xC0) ^ 0x80) && 
			!((str[2] & 0xC0) ^ 0x80) && 
			!((str[3] & 0xC0) ^ 0x80))
	{
		return 4;
	}
	/** Error encoded. */
	else
	{
		return -1;
	}
}

const char g_punctuation_array[][8] = {
	" ", "(", ")", "[", "]", 
	"{", "}", ";", "\"", "'", 
	"/", "\\", "=", "_", "*", 
	"&", "^", "$", "#", "@", 
	"!", "~", "`", "<", ">", 
	"?", "|", "　", "（", "）", 
	"［", "］", "｛", "｝", "，", 
	"。", "：", "；", "“", "”", 
	"‘", "’", "／", "、", "＋", 
	"＝", "－", "＿", "＊", "＆", 
	"…", "％", "＄", "＃", "＠", 
	"！", "～", "｀", "＜", "＞", 
	"《", "》", "？", "｜", "┊" };

map_t g_punctuation_map = NULL;

map_t init_punctuation_map(void)
{
	fprintf(stdout, "[INFO] -- init_punctuation_map now...\n");
	
	map_t m = hashmap_new ();
	
	int i = 0;
	int n = (sizeof(g_punctuation_array)/sizeof(g_punctuation_array[0]));
	for (i = 0; i < n; i ++)
	{
		if(MAP_OK != hashmap_put(m, g_punctuation_array[i], 
			strlen(g_punctuation_array[i]), (any_t)(g_punctuation_array[i]) ))
		{
			printf("[ERROR] -- %s:%d hashmap_put failed!\n", __FILE__, __LINE__);
			return NULL;
		}
	}
	
	return m;
}

/** Returns next token.
 * string	current data pointer
 * string_end	pointer one character behind the end of data
 * token_start	output hash value of the token
 * token_length
 */
static bool get_next_token_utf8( const char **string, const char *string_end, const char **token_start, size_t *token_length)
{
	// Skip white space
	int n_atom_len = 0;
	const char *start = *string;
	while (start != string_end && (n_atom_len = GetFirstAtomsLengthUTF8(start, string_end)) > 0)
	{
		if ( 1 == n_atom_len && isspace(start[0]) )
		{
			start ++;
			continue;
		}
		else if (3 == n_atom_len && 0 == strncmp(start, "　", 3))
		{
			start += 3;
			continue;
		}
		else if (MAP_OK == hashmap_is_exist(g_punctuation_map, start, n_atom_len))
		{
			start += n_atom_len;
			continue;
		}
		
		break;
	}
	
	/* At the end of the string? */
	if (start == string_end || 0 >= n_atom_len) {
		*string = start;
		dprintf( " token: end\n");
		return false;
	}
	
	*string = start+n_atom_len;
	*token_start = start;
	*token_length = n_atom_len;
	return true;
}



/** Hashes the given token.
 */
static uint64_t
get_hashed_token( const char *token_start, size_t token_length)
{
	uint32_t h1 = 0xac867c1d; /* Dummy init values */
	uint32_t h2 = 0x5434e4c4;
	hashlittle2( token_start, token_length, &h1, &h2);
	return ((uint64_t)h1 << 32) +h2;
}


/** Updates histogram with weight==1.
 * Tricky implementation, optimized for speed.
 */
static inline void
update_hist( int *hist, uint64_t token)
{
	uint32_t t = token;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1;
	t = token>>32;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist++ += t & 1; t >>= 1;
	*hist   += t & 1;
}


/** Calculates the hamming distance.
 */
int
hamming_dist( uint64_t a1, uint64_t a2)
{
	uint32_t v1 = a1^a2;
	uint32_t v2 = (a1^a2)>>32;

	v1 = v1 - ((v1>>1) & 0x55555555);
	v2 = v2 - ((v2>>1) & 0x55555555);
	v1 = (v1 & 0x33333333) + ((v1>>2) & 0x33333333);
	v2 = (v2 & 0x33333333) + ((v2>>2) & 0x33333333);
	int c1 = (((v1 + (v1>>4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
	int c2 = (((v2 + (v2>>4)) & 0x0F0F0F0F) * 0x01010101) >> 24;

	return c1+c2;
}


/** Calculates the similarity hash.
 */
uint64_t
charikar_hash64( const char *data, long data_length)
{
	const char *data_end = data +data_length;

	/* Clear histogram */
	int hist[ 64];
	memset( hist, 0, sizeof( hist));

	/* Token position and length */
	const char *token_start;
	size_t token_length;

	/* Over all tokens... */
	int tokens = 0;
	while ((get_next_token_utf8( &data, data_end, &token_start, &token_length))) {
		/* Calculate token hash */
		uint64_t token = get_hashed_token( token_start, token_length);
		dprintf( " token: %016llx %s\n", (long long)token, string( token_start, token_length));

		/* Update histogram, weigth==1 */
		update_hist( hist, token);
		tokens++;
	}
	tokens /= 2;
	// if token is 1, then set mid token line to 1. (add by firstboy)
	tokens = ((0 != tokens) ? tokens : 1);
	
	/* Calculate a bit vector from the histogram */
	uint64_t simhash=0;
	int c = 0;
	for (c=0; c<64; c++)
		simhash |= (uint64_t)((hist[ c]-tokens)>=0) << c;

	dprintf( " simhash: %016llx\n", simhash);
	return simhash;
}
