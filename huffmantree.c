// #define DEBUG

#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
        #include <stdio.h>
#endif

#include "huffmantree.h"
#include "heap/heap.h"

#define MAX_NODES_FACTOR 2
struct treenode *codes [NUMBER_OF_CHARS] = {0};

int compare_freqs(void* n1, void* n2) {
        struct treenode *n1_ptr = (struct treenode*) n1;
        struct treenode *n2_ptr = (struct treenode*) n2;

        return n2_ptr->freq - n1_ptr->freq;
}

struct treenode* create_internal_node(struct treenode *n1, struct treenode *n2) {
        struct treenode* new_node = malloc(sizeof(struct treenode));
        
        new_node->character = 0;
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

void traverse(struct treenode *root) {
        if (root->zero) {
                if (root->nbits > 0)
                        (root->zero)->n = root->n << 1;

                (root->zero)->nbits = root->nbits + 1;
                traverse(root->zero);
        }
        
#ifdef DEBUG
        printf("node = %p\n", root);
        printf("zero = %p\n", root->zero);
        printf("one = %p\n", root->one);
        printf("char = %c\n", root->character);
        printf("freq = %d\n", root->freq);
        printf("nbits = %d\n", root->nbits);
        printf("n = x%04lx\n", root->n);
        printf("--------------------------------------\n");
#endif
        if (root->zero == NULL && root->one == NULL) {
                int i = (int) root->character;
                codes[i] = root;
        }
                
        if (root->one) {
                if (root->nbits > 0)
                        (root->one)->n = root->n << 1;
                
                (root->one)->n += 1;
                (root->one)->nbits = root->nbits + 1;
                traverse(root->one);
        }
}

struct treenode* build_codes(int *frequencies) {
        struct treenode *root = build_tree(frequencies);
        traverse(root);

        return root;
}

struct treenode* encode_char(char c) {
        int i = (int) c;
        return codes[i];
}

struct treenode* decode_bit(struct treenode *root, unsigned short bit) {
        assert(bit == 1 || bit == 0);

        struct treenode *next;
        
        if (bit == 1) {
                if (root->one == NULL)
                        return NULL;
                
                next = root->one;
        } else {
                if (root->zero == NULL)
                        return NULL;
                
                next = root->zero;
        }

        return next;
}

unsigned short msb(long unsigned int n, int nbits, int bit_position) {
        int size = sizeof(long unsigned int) * 8; /* 8 bits per byte */
        long unsigned int tmp = n << (size - nbits + bit_position - 1);

        return (unsigned short) (tmp >> (size - 1));
}