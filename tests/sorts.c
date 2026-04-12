#include <rob/sorts.h>

#include <stdio.h>
#include <stdlib.h>


static void test_sort(rob_sort_t sort);


int main(int argc, char* argv[]) {

    if (argc < 2) return 0;

    int choice = atoi(argv[1]);

    switch (choice) {

        case 0: 

            printf("Testing Selection Sort.\n");
        
            test_sort(rob_selection_sort); 
            
            break;

        case 1: 
            
            printf("Testing Insertion Sort.\n");
        
            test_sort(rob_insertion_sort); 
            
            break;

        case 2: 

            printf("Testing Bubble Sort.\n");
        
            test_sort(rob_bubble_sort); 
            
            break;

        case 3: 
        
            printf("Testing Merge Sort.\n");
        
            test_sort(rob_merge_sort); 
            
            break;

        case 4: 
        
            printf("Testing Quick Sort.\n");    

            test_sort(rob_quick_sort); 
            
            break;
    }

    return 0;
}

static void test_sort(rob_sort_t sort) {

    int nums[] = {5, 1, 4, 2, 8, 1, 56, 2, -1};

    size_t size = sizeof nums[0];

    size_t n = sizeof nums / size;

    sort(nums, n, size, rob_int_cmp);

    for (size_t i = 0; i < n; i++) 
        printf("%d ", nums[i]);  
}