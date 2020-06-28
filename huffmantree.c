#include <stdlib.h>
#include <assert.h>

#include "huffmantree.h"
#include "heap/heap.h"

#define NUMBER_OF_CHARS 256
#define MAX_NODES_FACTOR 2

int compare_freqs(void* n1, void* n2) {
        struct treenode *n1_ptr = (struct treenode*) n1;
        struct treenode *n2_ptr = (struct treenode*) n2;

        return n1_ptr->freq - n2_ptr->freq;
}

struct treenode* create_internal_node(struct treenode *n1, struct treenode *n2) {
        struct treenode* new_node = malloc (sizeof(struct treenode));
        
        new_node->character = '\0';
        new_node->freq = n1->freq + n2->freq;
        new_node->n = 0;
        new_node->nbits = 0;
        new_node->zero = n1;
        new_node->one = n2;

        return new_node;
}

struct treenode* create_leaf(int character, int freq) {
        struct treenode *leaf = malloc(sizeof(struct treenode));

        leaf->character = character;
        leaf->freq = freq;
        leaf->n = 0;
        leaf->nbits = 0;
        leaf->one = NULL;
        leaf->zero = NULL;

        return leaf;
}




/* given an array of ints representing frequencies indexed by ascii code
 * build a huffman tree returning its root
 * returns tree root and array of leaves to use for decoding in constant time
 */ 
struct treenode* build_tree(int *frequencies) {
        struct heap *h = create_heap(MAX_NODES_FACTOR * NUMBER_OF_CHARS, compare_freqs);

        /* populate heap with all leaves */
        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (frequencies[i] > 0) {
                        struct treenode *n = create_leaf(i, frequencies[i]);
                
                        int ret = add_element(&h, (void *) n);
                        assert(ret != -1);
                }
        }

        /* combine nodes with minumum freqs until only one node remains */
        while (h->size > 1) {
                struct treenode *min_1 = (struct treenode*) remove_min(&h);
                assert(min_1);

                struct treenode *min_2 = (struct treenode*) remove_min(&h);
                assert(min_2);

                struct treenode *new_parent = create_internal_node(min_1, min_2);
                int ret = add_element(&h, (void *) new_parent);
                assert(ret != -1);
        }

        struct treenode *root = remove_min(&h);
        assert(root);
        
        destroy_heap(&h);

        return root; 
}
