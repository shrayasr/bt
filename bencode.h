#ifndef BENCODE_H_INCLUDED
#define BENCODE_H_INCLUDED

enum benc_t { benc_int, benc_str, benc_dict };

struct benc {
	benc_t type;
	union {
		int i;
		char *s;
		dict *d;
	} d;
};

// dict is an array of key-value pairs
// can dict contain other dicts?



#endif
