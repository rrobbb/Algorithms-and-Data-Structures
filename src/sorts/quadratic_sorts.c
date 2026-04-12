#include <rob/sorts.h>

#include <stdlib.h>
#include <string.h>


void rob_selection_sort(void *base, size_t n, size_t size, rob_comparator_t cmp) {

    if (!base || n < 2 || size == 0 || !cmp) return;

    for (size_t i = 0; i < n - 1; i++) {

        size_t min = i;

        for (size_t j = i + 1; j < n; j++) 
            if (cmp(rob_get_element(base, min, size), rob_get_element(base, j, size)) > 0)
                min = j;
            
        if (min != i) rob_swap(rob_get_element(base, min, size), rob_get_element(base, i, size), size);
    }
}

void rob_insertion_sort(void* base, size_t n, size_t size, rob_comparator_t cmp) {

    if (!base || n < 2) return;

    for (size_t i = 1; i < n; i++)
        for (size_t j = i; j > 0 && cmp(rob_get_element(base, j, size), rob_get_element(base, j - 1, size)) < 0; --j)
            rob_swap(rob_get_element(base, j, size), rob_get_element(base, j - 1, size), size);
}

void rob_bubble_sort(void* base, size_t n, size_t size, rob_comparator_t cmp) {
    
    if (!base || n < 2) return;

    for (size_t i = 0; i < n - 1; i++) {

        int swapped = 0;

        for (size_t j = 0; j < n - i - 1; j++)
            if (cmp(rob_get_element(base, j, size), rob_get_element(base, j + 1, size)) > 0) {
                rob_swap(rob_get_element(base, j, size), rob_get_element(base, j + 1, size), size);
                swapped = 1;
            }

        if (!swapped) break;
    }
}

void rob_cocktail_sort(void* base, size_t n, size_t size, rob_comparator_t cmp) {

    if (!base || n < 2 || size == 0 || !cmp) return;

    int swapped = 1;

    unsigned char *ptr = base;

    size_t start = 0, end = n - 1;

    while (swapped) {

        for (size_t i = start; i < end; i++) {

            unsigned char *current = rob_get_element(base, i, size);
            unsigned char *next = rob_get_element(base, i + 1, size);

            if (cmp(current, next) > 0) {
                rob_swap(current, next, size);
                swapped = 1;
            }
        }

        if (!swapped) break;

        swapped = 0;
        end--;

        for (size_t i = end; i > start; i--) {

            unsigned char *current = rob_get_element(base, i, size);
            unsigned char *next = rob_get_element(base, i - 1, size);

            if (cmp(current, next) < 0) {
                rob_swap(current, next, size);
                swapped = 1;
            }
            
        }

        start++;
    }
}

void rob_shell_sort(void* base, size_t n, size_t size, rob_comparator_t cmp) {
    
    if (!base || n < 2 || size == 0 || !cmp) return;

    unsigned char *ptr = (unsigned char *)base;

    void *tmp = malloc(size);
    
    if (!tmp) return;

    // 1. Ciclo per il gap (inizia da n/2 e si dimezza ogni volta)
    for (size_t gap = n / 2; gap > 0; gap /= 2) {
        
        // 2. Esegui un Insertion Sort "a salti" per questo gap
        for (size_t i = gap; i < n; i++) {
            memcpy(tmp, ptr + i * size, size);
            
            size_t j = i;
            
            // 3. Confronta elementi distanti 'gap' posizioni
            // Usiamo j >= gap per evitare l'underflow di size_t
            while (j >= gap && cmp(ptr + (j - gap) * size, tmp) > 0) {
                memcpy(ptr + j * size, ptr + (j - gap) * size, size);
                j -= gap;
            }
            
            memcpy(ptr + j * size, tmp, size);
        }
    }

    free(tmp);
}


/* LINEAR SORTS */

void rob_counting_sort(int A[], size_t n) {

    int* B = calloc(n, sizeof(int));

    for (size_t i = 0; i < n; i++) B[A[i]]++;

    size_t j = 0;

    for (size_t i = 0; i < n; i++)
        while (B[i] > 0) {
            A[j++] = i;
            B[i]--;
        }

    free(B);
}

void rob_radix_sort(int A[], size_t n) {

    int max = A[0];

    for (int i = 1; i < n; i++)
        if (A[i] > max) max = A[i];

    for (int exp = 1; max / exp > 0; exp *= 10)
        rob_bucket_sort(A, n, exp);
}

void rob_bucket_sort(int A[], size_t n, size_t exp) {

    int B[n];

    int i, bucket[10] = {0};

    for (size_t i = 0; i < n; i++) {
        int cifra = (A[i] / exp) % 10;
        bucket[cifra]++;
    }

    for (i = 1; i < 10; i++)
        bucket[i] += bucket[i - 1];

    for (i = n - 1; i >= 0; i--) {

        int cifra = (A[i] / exp) % 10;

        B[bucket[cifra] - 1] = A[i];
        bucket[cifra]--;
    }

    for (i = 0; i < n; i++) A[i] = B[i];
}