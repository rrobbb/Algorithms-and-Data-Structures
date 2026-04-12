#include <rob/sorts.h>

#include <stddef.h>
#include <stdlib.h>

#define CUTOFF 10


static void rob_quick_sort_recursive(void* base, size_t low, size_t high, size_t size, rob_comparator_t cmp);

static size_t rob_partition(void *base, size_t low, size_t high, size_t size, rob_comparator_t cmp);


void rob_quick_sort(void* base, size_t n, size_t size, rob_comparator_t cmp) {

    if (n < 2 || !base || !cmp || size == 0) return;

    rob_quick_sort_recursive(base, 0, n - 1, size, cmp);
}


static void rob_quick_sort_recursive(void* base, size_t low, size_t high, size_t size, rob_comparator_t cmp) {

    if (low >= high) return;

    if (high - low < CUTOFF) {

        size_t n = high - low + 1;

        rob_insertion_sort(rob_get_element(base, low, size), n, size, cmp);

        return;
    }

    size_t mid = rob_partition(base, low, high, size, cmp);

    rob_quick_sort_recursive(base, low, mid, size, cmp);
    rob_quick_sort_recursive(base, mid + 1, high, size, cmp);
}

static size_t rob_partition(void *base, size_t low, size_t high, size_t size, rob_comparator_t cmp) {

    unsigned char pivot[size];

    rob_copy(pivot, rob_get_element(base, low, size), size);

    size_t left = low - 1, right = high + 1;

    while (1) {

        do left++; while (cmp(rob_get_element(base, left, size), pivot) < 0);

        do right--; while (cmp(rob_get_element(base, right, size), pivot) > 0);

        if (left >= right) return right;

        rob_swap(rob_get_element(base, left, size), rob_get_element(base, right, size), size);
    }
}


static size_t rob_partition_median3(void *base, size_t low, size_t high, size_t size, rob_comparator_t cmp) {

    size_t mid = low + (high - low) / 2;

    if (cmp(base + low * size, base + mid * size) > 0) 
        rob_swap(base + low * size, base + mid * size, size);

    if (cmp(base + low * size, base + high * size) > 0) 
        rob_swap(base + low * size, base + high * size, size);

    if (cmp(base + mid * size, base + high * size) > 0) 
        rob_swap(base + mid * size, base + high * size, size);

    rob_swap(base + low * size, base + mid * size, size);

    return rob_partition(base, low, high, size, cmp);
}

void rob_quick_sort_median3(void *base, size_t low, size_t high, size_t size, rob_comparator_t cmp) {

    if (low >= high) return;

    size_t mid = rob_partition_median3(base, low, high, size, cmp);

    rob_quick_sort_median3(base, low, mid, size, cmp);
    rob_quick_sort_median3(base, mid + 1, high, size, cmp);
}


// EXERCISES

static size_t rob_partition_random(void *base, size_t low, size_t high, size_t size, rob_comparator_t cmp) {

    rob_swap(rob_get_element(base, low, size), rob_get_element(base, rand() % (high - low + 1) + low, size), size);

    unsigned char pivot[size];

    rob_copy(pivot, rob_get_element(base, low, size), size);

    size_t left = low - 1, right = high + 1;

    while (1) {

        do left++; while (cmp(rob_get_element(base, left, size), pivot) < 0);

        do right--; while (cmp(rob_get_element(base, right, size), pivot) > 0);

        if (left >= right) return right;

        rob_swap(rob_get_element(base, left, size), rob_get_element(base, right, size), size);
    }
}