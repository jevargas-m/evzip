#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

struct treenode {
        int nbits;
        long unsigned int n;
        char character;
        unsigned int freq;
        struct treenode *zero;
        struct treenode *one;
};

#define NUMBER_OF_CHARS 256

/* add a bit and return next node in the tree, if its a leaf
 * zero and one pointers will be null and nbits/n/character
 *  contain relevant values
 *  return null pointer if trying to decode something inexistent
 */ 
struct treenode* decode_bit(struct treenode *root, unsigned short bit);

/* given a char and a huffman tree return a leaf node with 
 * its proper encoding
 * return: NULL if char is not a leaf on the tree
 */ 
struct treenode* encode_char(char c);

void build_codes(int *frequencies);

#endif