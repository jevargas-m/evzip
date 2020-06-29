// #define DEBUG

#include <stdio.h>
#include <assert.h>

#include "../huffmantree.h"

int main() {
        int freqs [NUMBER_OF_CHARS] = {0};

        freqs['a'] = 15;
        freqs['b'] = 5;
        freqs['c'] = 100;
        freqs['d'] = 10;
        freqs['E'] = 20;
        freqs['f'] = 10;
        freqs['g'] = 15;
        freqs['h'] = 15;

        struct treenode *root = build_codes(freqs);

        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (freqs[i]) {
                        /* test encode and print results */
                        struct treenode *code = encode_char((char) i);
                        printf("%c : nbits = %d, n = x%04lx\n", i, code->nbits, code->n);

                        /* test proper decoding */
                        struct treenode *r = root;
                        for (int p = 1; p <= code->nbits; p++) {
                               unsigned short bit = msb(code->n, code->nbits, p);
        #ifdef DEBUG
                printf("msb = %d\n", bit);
        #endif
                               r = decode_bit(r, bit); 
                        }
                        assert(r->character == (char) i);
                }
        }

        return 0;
}