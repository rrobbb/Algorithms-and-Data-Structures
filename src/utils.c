#include <rob/utils.h>

#include <stdio.h>

void rob_print_int_string(const void *key, const void *value) {
    if (!key) return;
    
    const int k = *(const int*) key;
    const char *v = (const char*) value;

    if (v) printf("(%d: %s)", k, v);

    else printf("(%d)", k);
}

void rob_copy(void* dest, const void* src, const size_t size) {

    unsigned char* cdest = dest;

    const unsigned char* csrc = src;

    for (size_t i = 0; i < size; i++) cdest[i] = csrc[i];
}

void rob_swap(void* x, void* y, const size_t size) {

    unsigned char *cx = x, *cy = y;

    for (size_t i = 0; i < size; i++) {
        unsigned char temp = cx[i];
        cx[i] = cy[i];
        cy[i] = temp;
    }
}

int rob_int_cmp(const void* x, const void* y) { return *(int*) x - *(int*) y; }

size_t rob_int_hash(const void* key, const size_t m) { return (size_t) (*(const int*) key % m); }