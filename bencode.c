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

static int read_dict(struct benc **b, char *in, char terminator)
{
	unsigned char c;
	struct benc *key, *value;
	struct dict *d;
	int count = 0;
	int len = 0;

	(*b)->type = benc_dict;
	c = *in;
	d = (struct dict *)malloc(sizeof(struct dict));
	(*b)->d.d = d;
	
	while (c != terminator) {
		key = (struct benc *)malloc(sizeof(struct benc));
		len = benc_decode_full(in, &key);

		count += len;
		in += len;
		
		(*b)->d.d->key = key->d.s;

		value = (struct benc *)malloc(sizeof(struct benc));
		len = benc_decode_full(in, &value);

		count += len;
		in += len;

		(*b)->d.d->value = value;

		c = *in++;
		count++;
	}

	return count;
}

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
		return 1 + read_dict(b, in, 'e');
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
	struct dict *d;
	
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
		d = b->d.d;
		printf("{ %s : ", d->key);
		benc_print(d->value);
		printf(" }");
		break;
	case benc_invalid:
		break;
	}
}