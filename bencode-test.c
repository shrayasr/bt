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
     benc_free(b);

     // number test 2
     in = "i21e";
     b = benc_decode(in);
     assert(b->type == benc_int);
     assert(b->d.i == 21);
     benc_free(b);

     // number test 3
     in = "i-3e";
     b = benc_decode(in);
     assert(b->type == benc_int);
     assert(b->d.i == -3);
     benc_free(b);

     // number test 4
     in = "i0e";
     b = benc_decode(in);
     assert(b->type == benc_int);
     assert(b->d.i == 0);
     benc_free(b);

     // number test 5
     in = "i-0e";
     b = benc_decode(in);
     assert(b->type == benc_invalid);
     benc_free(b);

     // string test 1
     in = "4:spam";
     b = benc_decode(in);
     assert(b->type == benc_str);
     assert(strcmp(b->d.s, "spam") == 0);
     benc_free(b);

     // string test 2
     in = "3:bob";
     b = benc_decode(in);
     assert(b->type == benc_str);
     assert(strcmp(b->d.s, "bob") == 0);
     benc_free(b);

     // string test 3
     in = "15:www.example.com";
     b = benc_decode(in);
     assert(b->type == benc_str);
     assert(strcmp(b->d.s, "www.example.com") == 0);
     benc_free(b);

     // list test 1
     in = "l4:spame";
     b = benc_decode(in);
     assert(b->type == benc_list);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // list test 2
     in = "l4:spam4:eggse";
     b = benc_decode(in);
     assert(b->type == benc_list);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // list test 3
     in = "l4:spami3ee";
     b = benc_decode(in);
     assert(b->type == benc_list);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // dict test 1 { "foo" : 42 }
     in = "d3:fooi42ee";
     b = benc_decode(in);
     assert(b->type = benc_dict);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // dict test 2 { "cow" => "moo" }
     in = "d3:cow3:mooe";
     b = benc_decode(in);
     assert(b->type = benc_dict);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // dict test 3 { "cow" => "moo", "spam" => "eggs" }
     in = "d3:cow3:moo4:spam4:eggse";
     b = benc_decode(in);
     assert(b->type = benc_dict);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // dict test 4  { "spam" => [ "a", "b" ] }
     in = "d4:spaml1:a1:bee";
     b = benc_decode(in);
     assert(b->type = benc_dict);
     benc_print(b);
     printf("\n");
     benc_free(b);

     // dict test 5 { "publisher" => "bob", "publisher-webpage" => "www.example.com", "publisher.location" => "home" }
     in = "d9:publisher3:bob17:publisher-webpage15:www.example.com18:publisher.location4:homee";
     b = benc_decode(in);
     assert(b->type = benc_dict);
     benc_print(b);
     printf("\n");
     benc_free(b);
}
