/* comment out following line to print a summary of codes in the tree */
/* #define VERBOSE */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "huffmantree.h"

void print_codes(struct treenode *root, int *freqs) 
{
        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (freqs[i]) {
                        /* test encode and print results */
                        struct treenode *code = encode_char((unsigned char) i);
                        printf("%c(%d)\t: freq = %8d\tnbits = %d\tcode = x%04lx ",
                               i, i, freqs[i], code->nbits, code->code);

                        /* print bits on decoding mode */
                        struct treenode *r = root;
                        for (int p = 1; p <= code->nbits; p++) {
                               unsigned short bit = get_i_bit(code->code,
                                                              code->nbits, p);
                               printf("%d", bit);
                               r = decode_bit(r, bit); 
                        }
                        printf("\n");
                }
        }
}

int main(int argc, char **argv)
{
        int freqs [NUMBER_OF_CHARS] = {0};
        int flag_verbose = 0;
        int opt;
        
        while ((opt = getopt(argc, argv, "v")) != -1) {
                switch (opt) {
                case 'v':
                        flag_verbose = 1;
                        break;
                default:
                        goto error_usage;
                }
        }

        if (optind >= argc)
                goto error_usage;
        
        char *src_filename = argv[optind] ;
        
        FILE *src_file = fopen(src_filename, "rb");
        if (src_file == NULL)
                goto error_file;

        unsigned char c;
        while ((fread(&c, sizeof(c), 1, src_file)) == 1)
                freqs[(int) c]++; /* cast to avoid warning */
        
        struct treenode *root = build_codes(freqs);

        if (flag_verbose)
                print_codes(root, freqs);

        char tgt_filename [100] = {0};
        strcat(tgt_filename, src_filename);
        strcat(tgt_filename, ".ez");
        FILE *tgt_file = fopen(tgt_filename, "wb"); 
        if (tgt_file == NULL)
                goto error_file;
        
        /* write headers */
        int code = 0xC0DE;
        if (fwrite(&code, sizeof(code),1 ,tgt_file) != 1)
                goto error_file;

        if (fwrite(freqs, sizeof(freqs[0]), NUMBER_OF_CHARS, tgt_file)
            != NUMBER_OF_CHARS)
                goto error_file;

        code = 0xDADA;
        if (fwrite(&code, sizeof(code), 1 ,tgt_file) != 1)
                goto error_file;

        if (fseek(src_file, 0, SEEK_SET) != 0)
                goto error_file;

        /* encode consecutive bits using a buffer for writting to file */
        int bit_counter = 0;
        unsigned char byte_buffer = 0;
        while ((fread(&c, sizeof(c), 1, src_file)) == 1) {
                struct treenode *t = encode_char(c);
                assert(t);
                
                for (int i = 1; i <= t->nbits; i++) {
                        unsigned char bit = get_i_bit(t->code, t->nbits, i);
                        byte_buffer += bit;
                        bit_counter++;
                        if (bit_counter == 8 * sizeof(byte_buffer)) {
                                fwrite(&byte_buffer, sizeof(byte_buffer), 1, tgt_file);
                                bit_counter = 0;
                                byte_buffer = 0;
                        }
                        byte_buffer = byte_buffer << 1;
                }
        }
        /* write residual bits in MSB-s with trailing 0 */
        if (bit_counter) {
                byte_buffer = byte_buffer << (8 * sizeof(byte_buffer) - bit_counter - 1);
                if (fwrite(&byte_buffer, sizeof(byte_buffer), 1, tgt_file) != 1)
                        goto error_file;
        }
                
        if (fclose(src_file) != 0)
                goto error_file;

        if (fclose(tgt_file) != 0)
                goto error_file;

        destroy_tree(&root);
        assert(root == NULL);

        exit(EXIT_SUCCESS);
        return 0;

error_file :
        perror("error: file could not be read/write");
	exit(EXIT_FAILURE);

error_usage :
        perror("usage: ./evzip [-v] <src file>");
	exit(EXIT_FAILURE);
}