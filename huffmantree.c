/* #define DEBUG */
#include <stdlib.h>
#include <assert.h>

#ifdef DEBUG
        #include <stdio.h>
#endif

#include "huffmantree.h"
#include "heap/heap.h"

/* used for memoization */
static struct treenode *codes [NUMBER_OF_CHARS] = {0};

int compare_freqs(void* n1, void* n2) 
{
        struct treenode *n1_ptr = (struct treenode*) n1;
        struct treenode *n2_ptr = (struct treenode*) n2;

        return n2_ptr->freq - n1_ptr->freq;
}

/* create a node that adds the freq of two nodes and attaches them
 * as children on zero and one
 */ 
struct treenode* create_internal_node(struct treenode *n1, struct treenode *n2)
{
        struct treenode* new_node = malloc(sizeof(struct treenode));
        assert(new_node);
        
        new_node->character = 0;
        new_node->freq = n1->freq + n2->freq;
        new_node->code = 0;
        new_node->nbits = 0;
        new_node->is_leaf = 0;
        new_node->zero = n1;
        new_node->one = n2;

        return new_node;
}

struct treenode* create_leaf(int character, int freq)
{
        struct treenode *leaf = malloc(sizeof(struct treenode));
        assert(leaf);

        leaf->character = character;
        leaf->freq = freq;
        leaf->code = 0;
        leaf->nbits = 0;
        leaf->is_leaf = 1;
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
        assert(heap);

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
                int ret = add_element(&heap, (void*) new_parent);
                assert(ret != -1);
        }

        struct treenode *root = remove_min(&heap);
        assert(root);

        destroy_heap(&heap);

        return root; 
}

/* in-order traversal populating codes */
void populate_codes(struct treenode *root)
{
        assert(root);
        if (root->zero) {
                /* if zero shift bits left only */
                if (root->nbits > 0)
                        (root->zero)->code = root->code << 1;

                (root->zero)->nbits = root->nbits + 1;
                populate_codes(root->zero);
        }
        
#ifdef DEBUG
        pretty_print_node(root);
#endif

        /* save memoized code */
        if (root->is_leaf)
                codes[(int) root->character] = root;
                
        if (root->one) {
                /* if one shift bits left and add 1 */
                if (root->nbits > 0)
                        (root->one)->code = root->code << 1;
                
                (root->one)->code++;
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

struct treenode* encode_char(unsigned char c)
{
        return codes[(int) c];
}

struct treenode* decode_bit(struct treenode *root, unsigned short bit)
{
        assert(bit == 1 || bit == 0);
        return bit ? root->one : root->zero;
}

unsigned char get_i_bit(long unsigned int n, int nbits, int i)
{
        assert(i <= nbits);
        int size = sizeof(n) * 8; /* 8 bits per byte */
        long unsigned int tmp = n << (size - nbits + i - 1);

        return (unsigned char) (tmp >> (size - 1));
}

void destroy_tree(struct treenode **root)
{
        if ((*root)->zero)
                destroy_tree(&(*root)->zero);

        if ((*root)->one)
                destroy_tree(&(*root)->one);
        
        free(*root);
        *root = NULL;
}