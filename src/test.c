#include <stdio.h>

int main(int argc, char* argv[])
{
	unsigned int v = 0xa7;
	unsigned int c = (v + (v >> 4) & 0x0F0F0F0F);
	printf("%x\n", c);
	return 0;
}

