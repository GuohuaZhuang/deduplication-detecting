
#include "split.h"
#include "common.h"
#include <stdio.h>

// A: 1010, B: 1011, C: 1100, D: 1101, E: 1110, F: 1111
// 0000 0000 0000 0000
// fff0 0000 0000 0000 head 12bit		fff0000000000000
// 000f ff80 0000 0000 1th 13bit		000fff8000000000
// 0000 007f fC00 0000 2th 13bit		0000007ffC000000
// 0000 0000 03ff e000 3th 13bit		0000000003ffe000
// 0000 0000 0000 1fff 4th 13bit		0000000000001fff

dup_index_t simhash_split_12_13_1(uint64_t hashcode)
{
	return ((hashcode & 0xffffff8000000000ULL) >> 39);
}

dup_index_t simhash_split_12_13_2(uint64_t hashcode)
{
	return ((hashcode & 0xfff0000000000000ULL) >> 39) | ((hashcode & 0x0000007ffC000000ULL) >> 26);
}

dup_index_t simhash_split_12_13_3(uint64_t hashcode)
{
	return ((hashcode & 0xfff0000000000000ULL) >> 39) | ((hashcode & 0x0000000003ffe000ULL) >> 13);
}

dup_index_t simhash_split_12_13_4(uint64_t hashcode)
{
	return ((hashcode & 0xfff0000000000000ULL) >> 39) | ((hashcode & 0x0000000000001fffULL));
}

dup_index_t simhash_split_13_13_1(uint64_t hashcode)
{
	return ((hashcode & 0x000fff8000000000ULL) >> 26) | ((hashcode & 0x0000007ffC000000ULL >> 26));
}

dup_index_t simhash_split_13_13_2(uint64_t hashcode)
{
	return ((hashcode & 0x000fff8000000000ULL) >> 26) | ((hashcode & 0x0000000003ffe000ULL >> 13));
}

dup_index_t simhash_split_13_13_3(uint64_t hashcode)
{
	return ((hashcode & 0x000fff8000000000ULL) >> 26) | ((hashcode & 0x0000000000001fffULL));
}

dup_index_t simhash_split_13_13_4(uint64_t hashcode)
{
	return ((hashcode & 0x0000007ffC000000ULL) >> 13) | ((hashcode & 0x0000000003ffe000ULL >> 13));
}

dup_index_t simhash_split_13_13_5(uint64_t hashcode)
{
	return ((hashcode & 0x0000007ffC000000ULL) >> 13) | ((hashcode & 0x0000000000001fffULL));
}

dup_index_t simhash_split_13_13_6(uint64_t hashcode)
{
	return (hashcode & 0x0000000003ffffffULL);
}

