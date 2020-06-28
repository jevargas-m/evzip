#include <stdio.h>
#include "../huffmantree.h"

int main() {
        int freqs [NUMBER_OF_CHARS] = {0};

        freqs['a'] = 15;
        freqs['b'] = 5;
        freqs['c'] = 100;
        freqs['d'] = 10;
        freqs['e'] = 20;
        freqs['f'] = 10;
        freqs['g'] = 15;
        freqs['h'] = 15;

        for (int i = 0; i < NUMBER_OF_CHARS; i++)
               codes[i] = 0; 

        struct treenode *root = build_tree(freqs);
        traverse(root);

        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (codes[i])
                        printf("%c : nbits = %d, n = x%04lx\n", i, codes[i]->nbits, codes[i]->n);
        }

        return 0;
}