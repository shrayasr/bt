#include "bencode.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	struct benc *b;
	FILE *in = fopen("str1.txt", "r");
	
	if (in == NULL) return EXIT_FAILURE;
	
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 3);
}