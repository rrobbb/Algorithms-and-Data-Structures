#include <rob/sorts.h>

#include <stddef.h>
#include <stdlib.h>


static void rob_merge_sort_recursive(void* base, size_t low, size_t high, size_t size, rob_comparator_t cmp, void *tmp);

static void rob_merge(void *base, size_t low, size_t mid, size_t high, size_t size, rob_comparator_t cmp, void *tmp);


void rob_merge_sort(void *base, size_t n, size_t size, rob_comparator_t cmp) {

    if (n < 2) return;

    unsigned char *tmp = malloc(n * size);

    if (!tmp) return;

    rob_merge_sort_recursive(base, 0, n - 1, size, cmp, tmp);

    free(tmp);
}


static void rob_merge_sort_recursive(void *base, size_t low, size_t high, size_t size, rob_comparator_t cmp, void *tmp) {

    if (low >= high) return;

    size_t mid = low + (high - low) / 2;

    rob_merge_sort_recursive(base, low, mid, size, cmp, tmp);
    rob_merge_sort_recursive(base, mid + 1, high, size, cmp, tmp);

    rob_merge(base, low, mid, high, size, cmp, tmp);
}

static void rob_merge(void *base, size_t low, size_t mid, size_t high, size_t size, rob_comparator_t cmp, void *tmp) {

    size_t i = low, j = mid + 1;

    unsigned char* lx = rob_get_element(base, i, size);
    unsigned char* rx = rob_get_element(base, j, size);
    unsigned char* dest = tmp;

    while (i <= mid && j <= high) {

        if (cmp(lx, rx) <= 0) {
            rob_copy(dest, lx, size);
            lx += size;
            i++;
        } else {
            rob_copy(dest, rx, size);
            rx += size;
            j++;
        }

        dest += size;
    }

    if (i <= mid) rob_copy(dest, lx, (mid - i + 1) * size);

    if (j <= high) rob_copy(dest, rx, (high - j + 1) * size);

    rob_copy(rob_get_element(base, low, size), tmp, (high - low + 1) * size);
}