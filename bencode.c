#include "bencode.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

static int benc_decode_full(char *in, struct benc **b);

static int read_number(struct benc **b, char *in, char terminator, int acc)
{
	unsigned char c;
	unsigned int count = 0;
	int int_val = acc;
	int sign = 1;

	c = *in++;
	count++;
	
	if (c == '-') {
	  sign = -1;
	  c = *in++;
	  count++;
	}

	while (c != terminator) {
		int_val = int_val * 10 + (c - '0');
		c = *in++;
		count++;
	}

	(*b)->d.i = int_val * sign;
	(*b)->type = benc_int;
	
	if (sign == -1 &&
	    int_val == 0)
	  (*b)->type = benc_invalid;
	
	return count;
}

static int read_string(struct benc **b, char *in, char terminator, int accumulator)
{
	struct benc *blen = (struct benc *)malloc(sizeof(struct benc));
	int len = 0;
	char *s = NULL; 
	size_t i = 0;
	int count = 0;
	
	count = read_number(&blen, in, terminator, accumulator);
	len = blen->d.i;
	s = (char *)malloc(sizeof(char) * (len + 1));

	in = in + count;
	for (i = 0; i < len; i++) {
		s[i] = *in++;
		count++;
	}
	s[i] = '\0';

	(*b)->type = benc_str;
	(*b)->d.s = s;

	free(blen);

	return count;
}

static int read_list(struct benc **bl, char *in, char terminator)
{
	unsigned char c;
	//struct benc *bl = (struct benc *)malloc(sizeof(struct benc));
	struct list *l;
	int count = 0;
	int len = 0;

	(*bl)->type = benc_list;
	l = (struct list *)malloc(sizeof(struct list));
	(*bl)->d.l = l;
	
	do {
		l->node = (struct benc *)malloc(sizeof(struct benc));
		len = benc_decode_full(in, &(l->node));
		count += len;
		in += len;
		
		c = *in++;
		count++;
		
		if (c == terminator)
			l->next = NULL;
		else {
			in--;
			count--;
			l->next = (struct list *)malloc(sizeof(struct list));
		}
		l = l->next;
	} while (c != terminator);


	return count;
}

#if 0
static struct benc *read_dict(FILE *in, char terminator)
{
	unsigned char c;
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	struct benc *key, *value;

	b->type = benc_dict;
	c = read_byte(in);
	printf("%s ...\n", c);
	while (c != terminator) {
		key = benc_decode(in);
		b->d.d->key = key->d.s;
		value = benc_decode(in);
		b->d.d->value = value;
		c = read_byte(in);
	}

	return b;
}
#endif

static int benc_decode_full(char *in, struct benc **b)
{
	unsigned char first = *in++;

	switch (first) {
	case 'i':
		return 1 + read_number(b, in, 'e', 0);
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
		return 1 + read_string(b, in, ':', first - '0');
	case 'l':
		return 1 + read_list(b, in, 'e');
	case 'd':
		// return read_dict(b, in, 'e');
		return 0;
	default:
		fprintf(stderr, "%s: stream possibly corrupt\n",
					"benc_decode_full");
		return -1;
	}
}

struct benc *benc_decode(char *in)
{
	struct benc *b = (struct benc *)malloc(sizeof(struct benc));
	
	if (strlen(in) < 1) {
		perror(strerror(errno));
		return NULL;
	}
	else {
		benc_decode_full(in, &b);
		return b;
	}
}

void benc_print(struct benc *b)
{
	struct list *l;
	
	switch(b->type) {
	case benc_int:
		printf("%s: %d", "number", b->d.i);
		break;
	case benc_str:
		printf("%s: %s", "string", b->d.s);
		break;
	case benc_list:
		printf("[");
		l = b->d.l;
		if (l == NULL) return;
		while (l != NULL) {
			benc_print(l->node);
			printf(", ");
			l = l->next;
		}
		printf("]");
		break;
	case benc_dict:
		break;
	case benc_invalid:
		break;
	}
}