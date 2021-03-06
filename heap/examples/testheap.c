#include <stdio.h>
#include <assert.h>
#include "../heap.h"

int cmp(void* e1, void* e2) {
        int* e1_int = (int*) e1;
        int* e2_int = (int*) e2;

        return *e2_int - *e1_int;
}

int main () {

        int my_ints [] = {3, 5, 10, 1, 30, 4, 20, -5, 100, 8, 25};
        int n = sizeof(my_ints) / sizeof(int);

        struct heap *my_heap = create_heap(n, cmp);

        

        for (int i = 0; i < n; i++)
                add_element(&my_heap, (void*) &my_ints[i]);
        
        assert(my_heap->size == n);
        assert(add_element(&my_heap, (void*) &my_ints[0]) == -1);

        printf("heap = ");
        for (int i = 1; i <= n; i++) {
                int *e_ptr = (int*) my_heap->array[i];
                printf("%d ", *e_ptr);
        }
        printf("\n");      

        printf("heapSort = ");
        for (int i = 0; i < n; i++) {
                int *e_ptr = (int*) remove_min(&my_heap);
                assert(my_heap->size == n - i - 1);
                printf("%d ", *e_ptr);
        }
        printf("\n");    

        assert (remove_min(&my_heap) == NULL );
        assert(my_heap->size == 0);
        
        printf("input = ");
        for (int i = 0; i < n; i++) {
                printf("%d ", my_ints[i]);
        }
        printf("\n");  

        destroy_heap(&my_heap);

        return 0;
}