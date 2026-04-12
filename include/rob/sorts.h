#ifndef ROB_SORTS_H
#define ROB_SORTS_H

#include <rob/utils.h>

#include <stddef.h>


typedef void (*rob_sort_t)(void* base, size_t n, size_t size, rob_comparator_t cmp);

/**
 * @defgroup    GenericSorts
 * @brief       Function that sorts any array type.
 * @param base  Pointer to the first element of the array.
 * @param n     Number of elements in the array.
 * @param size  Size in byte of each element.
 * @param cmp   Pointer to the compare function.
 * @{
 */

void rob_selection_sort(void* base, size_t n, size_t size, rob_comparator_t cmp);

void rob_insertion_sort(void* base, size_t n, size_t size, rob_comparator_t cmp);

void rob_bubble_sort(void* base, size_t n, size_t size, rob_comparator_t cmp);

void rob_merge_sort(void* base, size_t n, size_t size, rob_comparator_t cmp);

void rob_quick_sort(void* base, size_t n, size_t size, rob_comparator_t cmp);

/** @} */

#endif