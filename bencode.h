#ifndef BENCODE_H_INCLUDED
#define BENCODE_H_INCLUDED

#include <stdio.h>

enum benc_t { benc_int, benc_str, benc_list, benc_dict, benc_invalid };

typedef struct list {
     struct benc *node;
     struct list *next;
} list;

typedef struct dict {
     char *key;
     struct benc *value;
     struct dict *next;
} dict;

struct benc {
     enum benc_t type;
     union {
	  int i;
	  char *s;
	  list *l;
	  dict *d;
     } d;
};

// dict is an array of key-value pairs
// can dict contain other dicts?

extern struct benc *benc_decode(char *stream);
extern void benc_print(struct benc *b);
extern void benc_free(struct benc *b);

#endif
