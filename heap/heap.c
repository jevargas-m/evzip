/* Min heap implementation
 * Author: jevargas-m
 * implements functions in heap.h
 */

#include <stdlib.h>
#include <assert.h>
#include "heap.h"

void destroy_heap(struct heap **h)
{
        assert(*h);

        free((*h)->array);
        free(*h);
}

struct heap* create_heap(int max, int(*compare)(void*, void*))
{
        struct heap *h = malloc(sizeof(struct heap));
        if (h == NULL)
                return NULL;

        h->size = 0;
        h->max = max;
        h->compare = compare;
        h->array = malloc((max + 1) * sizeof(void*)); /* +1 as index 0 is unused */
        if (h->array == NULL)
                return NULL;

        return h;
}

void swap(struct heap *h, int i, int j)
{
        void* tmp = h->array[i];

        h->array[i] = h->array[j];
        h->array[j] = tmp;
}

void percolate_up(struct heap *h, int i)
{
        assert(h);
        
        int parent = (int) i / 2;

        while (i > 1 && h->compare(h->array[i], h->array[parent]) > 0) {
                swap(h, i, parent);
                i = parent;
                parent = (int) parent / 2;
        }
}

void percolate_down(struct heap *h, int i)
{
        assert(h);

        if (h->size < 2)
                return;
        
        int left, right, candidate;

        while ((left = 2 * i) <= h->size) {
                right = left + 1;
                
                if (right > h->size)
                        candidate = left;
                else if (h->compare(h->array[left], h->array[right]) > 0)
                        candidate = left;
                else
                        candidate = right;
                
                if (h->compare(h->array[candidate], h->array[i]) > 0)
                        swap(h, i, candidate);
                else 
                        break;

                i = candidate;
        } 
}

int add_element(struct heap **h, void *e)
{
        assert (*h);

        if ((*h)->size == (*h)->max)
                return -1;

        (*h)->size++;
        (*h)->array[(*h)->size] = e;
        percolate_up(*h, (*h)->size);

        return 0;
}

void* remove_min(struct heap **h)
{
        assert(h && *h);

        if ((*h)->size == 0)
                return NULL;
        
        void *min = (*h)->array[1];

        (*h)->array[1] = (*h)->array[(*h)->size];
        (*h)->size--;
        percolate_down(*h, 1);

        return min;
}