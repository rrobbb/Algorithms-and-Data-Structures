#ifndef ROB_HT_LINPROB_H
#define ROB_HT_LINPROB_H

#include <rob/utils.h>


struct rob_ht_linprob_s;

typedef struct rob_ht_linprob_s rob_ht_linprob_t;


/**
 * @defgroup        HT_LINPROB_API
 * @brief           Core management functions for hash table with linear probing.
 * @param ht        Pointer to the hash table instance.
 * @param key       The key used for identification/ordering.
 * @{
 */


/**
 * @param m         Capacity of the hash table
 * @param hasher    A custom hash function
 * @param cmp       A custom compare function
 * @return          A pointer to an empty hash table, or NULL on failure.
 */
rob_ht_linprob_t* rob_ht_linprob_create(size_t m, rob_hasher_t hasher, rob_comparator_t cmp);

/**
 * @param destroy_data Boolean flag: if true, internal data is deallocated.
 */
void rob_ht_linprob_destroy(rob_ht_linprob_t **ht, const int destroy_data);

/**
 * @param value     Value to insert
 * @param override  Boolean flag: if true and the key is already presente, it updates the value.
 */
void rob_ht_linprob_put(rob_ht_linprob_t *ht, void *key, void *value, const int override);

void rob_ht_linprob_remove(rob_ht_linprob_t *ht, void *key);

/**
 * @return The value corresponding to the key, if any
 */
const void* rob_ht_linprob_search(const rob_ht_linprob_t *ht, const void *key);

/**
 * @return The load factor of the hash table
 */
const float rob_ht_linprob_loadfactor(const rob_ht_linprob_t *ht);

/**
 * @param print A custom print function (type define in @ref utils.h)
 */
void rob_ht_linprob_print(const rob_ht_linprob_t *ht, rob_printer_t print);

/** @} */


// EXERCISES

void rob_ht_linprob_merge(rob_ht_linprob_t *dest, const rob_ht_linprob_t *src);

void rob_ht_linprob_difference(rob_ht_linprob_t *dest, const rob_ht_linprob_t *src);


#endif