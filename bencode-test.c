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
	char *in = "i3e";
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 3);

	// number test 2
	in = "i21e";
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 21);

	// number test 3
	in = "i-3e";
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == -3);

	// number test 4
	in = "i0e";
	b = benc_decode(in);
	assert(b->type == benc_int);
	assert(b->d.i == 0);

	// number test 5
	in = "i-0e";
	b = benc_decode(in);
	assert(b->type == benc_invalid);

	// string test 1
	in = "4:spam";
	b = benc_decode(in);
	assert(b->type == benc_str);
	assert(strcmp(b->d.s, "spam") == 0);

	// string test 2
	in = "3:bob";
	b = benc_decode(in);
	assert(b->type == benc_str);
	assert(strcmp(b->d.s, "bob") == 0);

	// string test 3
	in = "15:www.example.com";
	b = benc_decode(in);
	assert(b->type == benc_str);
	assert(strcmp(b->d.s, "www.example.com") == 0);

#if 0
	// list test 1
	in = "l4:spame";
	b = benc_decode(in);
	assert(b->type == benc_list);
	benc_print(b);
	printf("\n");

	// list test 2
	in = "l4:spam4:eggse";
	b = benc_decode(in);
	assert(b->type == benc_list);
	benc_print(b);
	printf("\n");
	fclose(in);

	// list test 3
	in = fmemopen("l4:spami3ee", 11, "r");
	b = benc_decode(in);
	assert(b->type == benc_list);
	benc_print(b);
	printf("\n");
	fclose(in);
#endif	
}
