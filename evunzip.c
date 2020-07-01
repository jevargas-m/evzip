#include <stdio.h>
#include <assert.h>

#include "huffmantree.h"

int main(int argc, char **argv)
{
        int freqs [NUMBER_OF_CHARS] = {0};
        int nchars, code;      
        
        if (argc != 3) {
		perror("error: usage: ./evunzip <source ez file> <destination file>\n");
		return -1;
	}
        char *filename = argv[1];
        char *dest_txt = argv[2];
        
        FILE *src_file = fopen(filename, "rb");
	if (src_file == NULL) {
		perror("open_file: could not open file\n");
		return -1;
	}

        /* read headers */
        fread(&code, sizeof(code), 1, src_file);
        assert(code == 0xc4a2b175);
        fread(&nchars, sizeof(nchars),1 ,src_file);
        fread(&code, sizeof(code), 1, src_file);
        assert(code == 0xC0DE);
        fread(freqs, sizeof(freqs[0]), NUMBER_OF_CHARS, src_file);  
        fread(&code, sizeof(code), 1, src_file);
        assert(code == 0xDADA); 
     
        /* build tree */
        struct treenode *root = build_codes(freqs);

        FILE *tgt_file = fopen(dest_txt, "w"); 

        int counter = 0;
        unsigned char code_in = 0;
        struct treenode *t = root;
        while ( fread(&code_in, sizeof(code_in), 1, src_file) == 1) {
                for (int i = 1; i <= 8; i++) {
                        unsigned char bit = get_i_bit((long unsigned int) code_in,
                                                       sizeof(code_in) * 8,
                                                       sizeof(long unsigned int) - 8 + i);
                        t = decode_bit(t, bit);
                        if (t == NULL)
                                break;
                        if (t->is_leaf) {
                                fputc(t->character, tgt_file);
                                counter++;
                                if (counter == nchars)
                                        break;
                                t = root;
                        }
                }
                
        }
        fclose(src_file);
        fclose(tgt_file);
        destroy_tree(&root);

        return 0;
}
