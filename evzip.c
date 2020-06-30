#define DEBUG

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
                        printf("%c\t: freq = %d\tnbits = %d\tn = x%04lx\t", i, freqs[i], code->nbits, code->code);

                        /* test proper decoding */
                        struct treenode *r = root;
                        for (int p = 1; p <= code->nbits; p++) {
                               unsigned short bit = get_i_bit(code->code, code->nbits, p);
                               printf("%d", bit);
                               r = decode_bit(r, bit); 
                        }
                        printf("\n");
                        assert(r->character == (char) i);
                }
        }

        f = fopen(filename, "r"); 
        char tgt_file [100] = {0};
        strcat(tgt_file, filename);
        strcat(tgt_file, ".evzip");
        FILE* tgt = fopen(tgt_file, "wb"); 

        int bit_counter = 0;
        int counter = 0;
        unsigned char out_char = 0;
        unsigned long int total_chars = 0;
        while ((c = fgetc(f)) != EOF) {
                counter++;
                total_chars++;
                struct treenode *t = encode_char(c);
                if (t == NULL) {
                        printf("ERROR: file not properly ASCII formated, %c at %d not recognized\n", c, counter);
                        printf("to convert try:\n");
                        printf("iconv -f utf-8 -t ascii//TRANSLIT \"source file\" > \"destination_file>\"\n");
                        return 1;
                }

                
                for (int i = 1; i <= t->nbits; i++) {
                        unsigned char bit = get_i_bit(t->code, t->nbits, i);
                        out_char += bit;
                        bit_counter++;
                        if (bit_counter == 8 * sizeof(out_char)) {
                                fwrite(&out_char, sizeof(out_char), 1, tgt);
                                bit_counter = 0;
                                out_char = 0;
                        }
                        out_char = out_char << 1;
                }
                
                
        }
        /* write residual bits in MSB-s with trailing 0 */
        if (bit_counter) {
                out_char = out_char << (8 * sizeof(out_char) - bit_counter - 1);
                fwrite(&out_char, sizeof(out_char), 1, tgt);
        }
                
        fclose(f);
        fclose(tgt);

        /* decode here */
        char dest_txt [100] = {0};
        strcat(dest_txt, filename);
        strcat(dest_txt, ".atxt");
        FILE *sf = fopen(tgt_file, "rb"); 
        FILE *tf = fopen(dest_txt, "w"); 

        bit_counter = 0;
        counter = 0;
        unsigned char code_in = 0;
        struct treenode *t = root;
        while ( fread(&code_in, sizeof(code_in), 1, sf) == 1) {
                
                for (int i = 1; i <= 8; i++) {
                        unsigned char bit = get_i_bit((long unsigned int) code_in, 8, sizeof(long unsigned int) - 8 + i);
                        t = decode_bit(t, bit);
                        if (t == NULL)
                                break;
                        if (t->one == NULL && t->zero == NULL) {
                                fputc(t->character, tf);
                                counter++;
                                if (counter == total_chars)
                                        break;
                                t = root;
                        }
                }
                
        }
        fclose(sf);
        fclose(tf);

        destroy_tree(&root);
        assert(root == NULL);

        return 0;
}
