#include "bencode.h"

#include <assert.h>
#include <stdlib.h>

struct benc *benc_decode(unsigned char *stream)
{
	
}

struct benc *benc_decode_str(unsigned char *stream)
{
	// "x:foo" where x is an integer indicating the length of "foo"
	size_t i;
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	char len = *stream;

	if (b == NULL) return NULL;

	i = atoi(len);
	assert(stream[1] == ':');

	// initialize the benc structure
	b->type = benc_str;
	b->d.s = (char *)malloc(sizeof(char) * (i + 1));

	strcpy(b->d.s, &stream[2], i);
	b->d.s[i] = '\0';

	return b;
}

struct benc *benc_decode_int(unsigned char *stream)
{
	// "ixe" where x is either a number without a negative sign, in which
	// case, it is a positive number, or it is a number with a negative sign.
	// The numbers should not have leading zeros.
	size_t i;
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	char len;
	int n = 1;
	int num = 0, x = 0;
	char sign;
		
	// first char is 'i'
	assert(stream[0] == 'i');
	
	// read the next characters until 'e'
	// read the first char. It is either a '-' or '0' - '9'
	sign = stream[n++];
	if (sign != '-')
		num = atoi(sign);
	while ((x = stream[n++]) != 'e') {
		num = num*10 + atoi(x);
	}
	b->type = benc_int;
	b->d.i = num;
	
	return b;
}