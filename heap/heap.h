/* Min heap interface
 * Author: jevargas-m
 *
 */

#ifndef _HEAP_H_
#define _HEAP_H_

struct heap {
        int size;                     /* current size */
        int max;                      /* max size */
        int (*compare)(void*, void*); /* compare function */
        void **array;                 /* array of pointers to data in heap */
};

/* Create an empty heap , must be used first
 *      max : maximum number of elements the heap can store
 *      compare : pointer to function that compares two elements
 *                
 *                 function returns > 0 if first element is larger than second
 *                                  <= 0 otherwise
 *      returns:  pointer to heap, NULL: error
 */ 
struct heap* create_heap(int max,  int (*compare)(void*, void*));

/* Free memory used by heap */
void destroy_heap(struct heap **h);

/* insert a new element 
 * returns 0: success,  -1: error, heap overflow
 */ 
int add_element(struct heap **h, void* e);

/* remove element smallest element according to compare function
 * returns:  pointer to max element,  NULL: if error or empty heap
 */ 
void* remove_min(struct heap **h); 

#endif