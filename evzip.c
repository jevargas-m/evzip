#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "huffmantree.h"

void print_codes(struct treenode *root, int *freqs) 
{
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
}

int main(int argc, char **argv)
{
        int freqs [NUMBER_OF_CHARS] = {0};
        
        /* read source file and populate frequencies */
        if (argc != 2) {
		perror("usage: ./evzip <text file>\n");
		return 1;
	}
        char *src_filename = argv[1] ;
        
        FILE *src_file = fopen(src_filename, "r");
	if (src_file == NULL) {
		perror("open_file: could not open file\n");
		return 1;
	}

        char c;
        while ((c = fgetc(src_file)) != EOF)
                freqs[(int) c]++; /* cast to avoid warning */
        
        fclose(src_file);
        
        /* build tree */
        struct treenode *root = build_codes(freqs);

        print_codes(root, freqs);

        char tgt_filename [100] = {0};
        strcat(tgt_filename, src_filename);
        strcat(tgt_filename, ".ez");
        FILE *tgt_file = fopen(tgt_filename, "wb"); 
        assert(tgt_file);
        
        src_file = fopen(src_filename, "r"); 
        int bit_counter = 0;
        int counter = 0;
        unsigned char out_char = 0;

        /* write headers */
        int code = 0xC0DE;
        fwrite(&code, sizeof(code),1 ,tgt_file);
        fwrite(freqs, sizeof(freqs[0]), NUMBER_OF_CHARS, tgt_file); 
        code = 0xDADA;
        fwrite(&code, sizeof(code), 1 ,tgt_file);

        while ((c = fgetc(src_file)) != EOF) {
                counter++;
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
                                fwrite(&out_char, sizeof(out_char), 1, tgt_file);
                                bit_counter = 0;
                                out_char = 0;
                        }
                        out_char = out_char << 1;
                }
        }
        /* write residual bits in MSB-s with trailing 0 */
        if (bit_counter) {
                out_char = out_char << (8 * sizeof(out_char) - bit_counter - 1);
                fwrite(&out_char, sizeof(out_char), 1, tgt_file);
        }
                
        fclose(src_file);
        fclose(tgt_file);
        destroy_tree(&root);
        assert(root == NULL);

        return 0;
}