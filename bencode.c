#include "bencode.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static struct benc *benc_decode_full(FILE *in);

static unsigned char read_byte(FILE *in)
{
	unsigned char c = fread((unsigned char *)&c, sizeof(unsigned char), 1, in);
	
	if (c == 1)
		return c;
	else {
		perror(strerror(errno));
		return 0;
	}
}

static struct benc *read_number(FILE *in, char terminator, int acc)
{
	unsigned char c;
	int int_val = acc;
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	
	c = read_byte(in);
	while (c != terminator) {
		int_val = int_val * 10 + (c - '0');
		c = read_byte(in);
	}
	b->type = benc_int;
	b->d.i = int_val;
	
	return b;
}

static struct benc *read_string(FILE *in, char terminator, int accumulator)
{
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	struct benc *blen = read_number(in, terminator, accumulator);
	int len = blen->d.i;
	char *s = (char *)malloc(sizeof(char) * (len + 1));
	size_t i = 0;
	
	for (i = 0; i < len; i++)
		s[i] = read_byte(in);
	s[i] = '\0';
	
	b->type = benc_str;
	b->d.s = s;

	return b;	
}

static struct benc *read_list(FILE *in, char terminator)
{
	unsigned char c;
	struct benc *bl = (struct benc *)malloc(sizeof(struct benc));
	struct benc **elem;
	
	bl->type = benc_list;
 	elem = &(bl->d.l->node);
	c = read_byte(in);
	
	while (c != terminator) {
		*elem = benc_decode_full(in);
		elem = &(bl->d.l->next);
		c = read_byte(in);
	}
	
	return bl;
}

static struct benc *read_dict(FILE *in, char terminator)
{
	unsigned char c;
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	struct benc *key, *value;

	b->type = benc_dict;
	c = read_byte(in);
	while (c != terminator) {
		key = benc_decode(in);
		b->d.d->key = key->d.s;
		value = benc_decode(in);
		b->d.d->value = value;
		c = read_byte(in);
	}

	return b;
}

static struct benc *benc_decode_full(FILE *in)
{
	unsigned char first = read_byte(in);
	
	switch (first) {
	case 'i':
		return read_number(in, 'e', 0);
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return read_string(in, ':', first - '0');
	case 'l':
		return read_list(in, 'e');
	case 'd':
		return read_dict(in, 'e');
	default:
		fprintf(stderr, "%s: stream possibly corrupt\n",
					"benc_decode_full");
		return NULL;
	}
}

struct benc *benc_decode(FILE *in)
{
	if (feof(in)) {
		perror(strerror(errno));
		return NULL;
	}
	else
		return benc_decode_full(in);
}