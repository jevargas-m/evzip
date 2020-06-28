#include <stdio.h>
#include <assert.h>
#include "../../heap.h"

int cmp(void* e1, void* e2) {
        int* e1_int = (int*) e1;
        int* e2_int = (int*) e2;

        return *e2_int - *e1_int;
}

int main () {

        int my_ints [] = {3, 5, 10, 1, 30, 4, 20, -5, 100, 8, 25};
        int n = 11;

        struct heap *my_heap = create_heap(n, cmp);

        

        for (int i = 0; i < n; i++)
                add_element(&my_heap, (void*) &my_ints[i]);
        
        assert(my_heap->size == n);

        printf("heap = ");
        for (int i = 1; i <= n; i++) {
                void **a = my_heap->array;
                int *e_ptr = (int*) a[i];
                printf("%d ", *e_ptr);
        }
        printf("\n");      

        printf("heapSort = ");
        for (int i = 0; i < n; i++) {
                int *e_ptr = (int*) remove_max(&my_heap);
                printf("%d ", *e_ptr);
        }
        printf("\n");     
        
        printf("input = ");
        for (int i = 0; i < n; i++) {
                printf("%d ", my_ints[i]);
        }
        printf("\n");  

        destroy_heap(&my_heap);

        return 0;
}