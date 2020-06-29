#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "huffmantree.h"

int main(int argc, char **argv)
{
        int freqs [NUMBER_OF_CHARS] = {0};      
        
        if (argc != 2) {    /* check if there is an argument present */
		perror("usage: ./evzip <text file>\n");
		return 1;
	}
        char* filename = argv[1] ;

        FILE* f = fopen(filename, "r"); /* create file handler */
	if (f == NULL) {
		perror("open_file: could not open file\n");
		return 1;
	}

        char c;
        while ((c = fgetc(f)) != EOF) {
                int i = (int) c;
                freqs[i]++;
        }

        fclose(f);

        struct treenode *root = build_codes(freqs);

        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (freqs[i]) {
                        /* test encode and print results */
                        struct treenode *code = encode_char((char) i);
                        printf("%c\t: freq = %d\tnbits = %d\tn = x%04lx\t", i, freqs[i], code->nbits, code->n);

                        /* test proper decoding */
                        struct treenode *r = root;
                        for (int p = 1; p <= code->nbits; p++) {
                               unsigned short bit = msb(code->n, code->nbits, p);
                               printf("%d", bit);
                               r = decode_bit(r, bit); 
                        }
                        printf("\n");
                        assert(r->character == (char) i);
                }
        }
        

        f = fopen(filename, "r"); 
        char tgt_file [100];
        strcat(tgt_file, filename);
        strcat(tgt_file, ".evzip");
        FILE* tgt = fopen(tgt_file, "wb"); 

        int bit_counter = 0;
        int counter = 0;
        char out_char = 0;
        while ((c = fgetc(f)) != EOF) {
                counter++;
                struct treenode *t = encode_char(c);
                if (t == NULL) {
                        printf("ERROR: file not properly ASCII formated, %c at %d not recognized\n", c, counter);
                        printf("to convert try:\n");
                        printf("iconv -f utf-8 -t ascii//TRANSLIT \"source file\" > \"destination_file>\"\n");
                        return 1;
                }

                
                for (int i = 1; i <= t->nbits; i++) {
                        unsigned short bit = msb(t->n, t->nbits, i);
                        
                        out_char += bit;
                        bit_counter++;
                        if (bit_counter == 8) {
                                fputc(out_char, tgt);
                                bit_counter = 0;
                                out_char = 0;
                        }
                        out_char = out_char << 1;
                }
                
                
        }
        fclose(f);
        fclose(tgt);

        
        
        return 0;
}
