#ifndef BENCODE_H_INCLUDED
#define BENCODE_H_INCLUDED

enum benc_t { benc_int, benc_str, benc_list, benc_dict };

struct benc {
	enum benc_t type;
	union {
		int i;
		char *s;
		// List *l;
		// Dict *d;
	} d;
};

// dict is an array of key-value pairs
// can dict contain other dicts?

extern struct benc *benc_decode_str(unsigned char *stream);
extern struct benc *benc_decode_int(unsigned char *stream);

#endif
