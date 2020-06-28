#include <stdlib.h>
#include <assert.h>

#include "huffmantree.h"

#define NUMBER_OF_CHARS 256

/* given an array of ints representing frequencies indexed by ascii code
 * build a huffman tree returning its root
 * returns tree root and array of leaves to use for decoding in constant time
 */ 
struct treenode* build_tree(int *frequencies, struct treenode **leaves) {
      *leaves = calloc(NUMBER_OF_CHARS, sizeof(struct treenode*));

      for (int i = 0; i < NUMBER_OF_CHARS; i++) {
              struct treenode *new_leaf = calloc(1, sizeof(struct treenode));

              new_leaf->character = i;
              new_leaf->freq = frequencies[i];
              leaves[i] = new_leaf;
      }

}

/* return index of node with minimum frequency, n points to number of nodes  */
int find_min (struct treenode **leaves, int *n) {
        struct treenode *min = NULL;
        int min_index = -1;
        int *n = 0;

        /* find first non-empty node */
        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (leaves[i]->freq != 0) {
                        min = leaves[i];
                        min_index = i;
                        break;
                }
        }
        assert(min);
        
        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (leaves[i]->freq > 0) {
                        *n = *n + 1;
                        if (leaves[i]->freq < min->freq) {
                                min = leaves[i];
                                min_index = i;
                        }
                }
        }
        
        return min_index;
}

/* combine the 2 minimum frequency nodes into a single node */
struct treenode* combine_mins (struct treenode **leaves) {
        



}

