#ifndef ROB_UTILS_H
#define ROB_UTILS_H

#include <stddef.h>


typedef void    (*rob_printer_t)        (const void *key, const void *value);
typedef void    (*rob_copy_t)           (void*, const void*, const size_t);
typedef int     (*rob_comparator_t)     (const void*, const void*);
typedef size_t  (*rob_hasher_t)         (const void*, const size_t);


/**
 * @defgroup            Utils
 * @brief               Utility functions.
 * @param size          Size in byes of the elements.
 * @param key           The key used for identification/ordering.
 * @{
 */

/**
 * @param base  Start of the array
 * @param index Position of the element
 * @return      The element in the index position
 */
static inline void* rob_get_element(void *base, const size_t index, const size_t size) { return (unsigned char*) base + index * size; }

/**
 * @param x first element
 * @param y second element
 */
void rob_print_int_string(const void *key, const void *value);

/**
 * @param dest  destination
 * @param src   source
 */
void rob_copy(void* dest, const void* src, const size_t size);

/**
 * @param x first element
 * @param y second element
 */
void rob_swap(void* x, void* y, const size_t size);

/**
 * @param x first element
 * @param y second element
 */
int rob_int_cmp(const void* x, const void* y);

/**
 * @param key   The key used for identification/ordering.
 * @param m     Capacity of the hast table.
 * @return      An index for the hash table.
 */
size_t rob_int_hash(const void* key, const size_t m);

/** @} */


#endif