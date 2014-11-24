#include "bencode.h"

#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	struct benc *b;
	char *s = "i3e";
	FILE *in = fmemopen(s, 3 * sizeof(char), "r");
	
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 3);
}