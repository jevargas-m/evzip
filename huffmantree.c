/* #define DEBUG */

#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
        #include <stdio.h>
#endif

#include "huffmantree.h"
#include "heap/heap.h"

struct treenode *codes [NUMBER_OF_CHARS] = {0};

int compare_freqs(void* n1, void* n2) 
{
        struct treenode *n1_ptr = (struct treenode*) n1;
        struct treenode *n2_ptr = (struct treenode*) n2;

        return n2_ptr->freq - n1_ptr->freq;
}

struct treenode* create_internal_node(struct treenode *n1, struct treenode *n2)
{
        struct treenode* new_node = malloc(sizeof(struct treenode));
        
        new_node->character = 0;
        new_node->freq = n1->freq + n2->freq;
        new_node->code = 0;
        new_node->nbits = 0;
        new_node->zero = n1;
        new_node->one = n2;

        return new_node;
}

struct treenode* create_leaf(int character, int freq)
{
        struct treenode *leaf = malloc(sizeof(struct treenode));

        leaf->character = character;
        leaf->freq = freq;
        leaf->code = 0;
        leaf->nbits = 0;
        leaf->one = NULL;
        leaf->zero = NULL;

        return leaf;
}

#ifdef DEBUG
void pretty_print_node(struct treenode *n)
{
        printf("node = %p\n", n);
        printf("zero = %p\n", n->zero);
        printf("one = %p\n", n->one);
        printf("char = %c\n", n->character);
        printf("freq = %d\n", n->freq);
        printf("nbits = %d\n", n->nbits);
        printf("n = x%04lx\n", n->n);
        printf("--------------------------------------\n");
}
#endif

/* given an array of ints representing frequencies indexed by ascii code
 * build a huffman tree returning its root
 */ 
struct treenode* build_tree(int *frequencies)
{
        /* full binary tree has at most 2n-1 nodes */
        struct heap *heap = create_heap((2 * NUMBER_OF_CHARS - 1), compare_freqs);

        /* populate heap with all leaves */
        for (int i = 0; i < NUMBER_OF_CHARS; i++) {
                if (frequencies[i] > 0) {
                        struct treenode *n = create_leaf(i, frequencies[i]);

                        int ret = add_element(&heap, (void*) n);
                        assert(ret != -1);
                }
        }

        /* combine nodes with minumum freqs until only one node remains */
        while (heap->size > 1) {
                struct treenode *min_1 = (struct treenode*) remove_min(&heap);
                assert(min_1);

                struct treenode *min_2 = (struct treenode*) remove_min(&heap);
                assert(min_2);

                struct treenode *new_parent = create_internal_node(min_1, min_2);
                int ret = add_element(&heap, (void *) new_parent);
                assert(ret != -1);
        }

        struct treenode *root = remove_min(&heap);
        destroy_heap(&heap);

        return root; 
}

/* in-order traversal populating codes */
void populate_codes(struct treenode *root)
{
        if (root->zero) {
                if (root->nbits > 0)
                        (root->zero)->code = root->code << 1;

                (root->zero)->nbits = root->nbits + 1;
                populate_codes(root->zero);
        }
        
#ifdef DEBUG
        pretty_print_node(root);
#endif

        if (root->zero == NULL && root->one == NULL) {
                int i = (int) root->character;
                codes[i] = root;
        }
                
        if (root->one) {
                if (root->nbits > 0)
                        (root->one)->code = root->code << 1;
                
                (root->one)->code += 1;
                (root->one)->nbits = root->nbits + 1;
                populate_codes(root->one);
        }
}

struct treenode* build_codes(int *frequencies)
{
        struct treenode *root = build_tree(frequencies);
        populate_codes(root);

        return root;
}

struct treenode* encode_char(char c)
{
        int i = (int) c;
        return codes[i];
}

struct treenode* decode_bit(struct treenode *root, unsigned short bit)
{
        assert(bit == 1 || bit == 0);

        if (bit == 1)
                return root->one ? root->one : NULL;
        else
                return root->zero ? root->zero : NULL;
}

unsigned char get_i_bit(long unsigned int n, int nbits, int i)
{
        assert(i <= nbits);
        int size = sizeof(n) * 8; /* 8 bits per byte */
        long unsigned int tmp = n << (size - nbits + i - 1);

        return (unsigned short) (tmp >> (size - 1));
}