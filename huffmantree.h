#ifndef __HUFFMANTREE_H__
#define __HUFFMANTREE_H__

struct treenode {
        int nbits;
        long unsigned int code;
        char character;
        unsigned int freq;
        int is_leaf;
        struct treenode *zero;
        struct treenode *one;
};

#define NUMBER_OF_CHARS 256

/* build huffman codes for an array of frequences indexed by ASCII code
 */
struct treenode* build_codes(int *frequencies);

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

/* get the i'th bit on a number n of nbits (start at 1 end at nbits)
 */
unsigned char get_i_bit(long unsigned int n, int nbits, int i);

void destroy_tree(struct treenode **root);

#endif