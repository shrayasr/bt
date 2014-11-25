#define _GNU_SOURCE
#include "bencode.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	struct benc *b;

	// number test 1
	FILE *in = fmemopen("i3e", 3, "r");
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 3);
	fclose(in);

	// number test 2
	in = fmemopen("i21e", 4, "r");
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 21);
	fclose(in);

	// number test 3
	in = fmemopen("i-3e", 4, "r");
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == -3);
	fclose(in);

	// number test 4
	in = fmemopen("i0e", 3, "r");
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 0);
	fclose(in);

	// number test 5
	in = fmemopen("i-0e", 4, "r");
	b = benc_decode(in);
	assert(b->type == benc_invalid);
	fclose(in);

	// string test 1
	in = fmemopen("4:spam", 6, "r");
	b = benc_decode(in);
	assert(b->type == benc_str);
	assert(strcmp(b->d.s, "spam") == 0);
	fclose(in);
}
