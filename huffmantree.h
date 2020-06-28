#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

struct treenode {
        int nbits;
        int n;
        char character;
        struct treenode *zero;
        struct treenode *one;
};

/* given an array of ints representing frequencies indexed by ascii code
 * build a huffman tree returning its root
 */ 
struct treenode* build_tree(int *frequencies);

/* add a bit and return next node in the tree, if its a leaf
 * zero and one pointers will be null and nbits/n/character
 *  contain relevant values
 */ 
struct treenode* decode_bit(struct treenode *t, unsigned short bit);

/* given a char and a huffman tree return a leaf node with 
 * its proper encoding
 */ 
struct treenode* encode_char(struct treenode *root, char c);

/* map tree leaves into an array of nodes indexed by ASCII code */
int* build_code_array(struct treenode *root);

#endif