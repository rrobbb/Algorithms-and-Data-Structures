#ifndef ROB_HT_SEPCHAIN_H
#define ROB_HT_SEPCHAIN_H

#include <rob/utils.h>


struct rob_ht_sepchain_s;

typedef struct rob_ht_sepchain_s rob_ht_sepchain_t;


/**
 * @defgroup        HT_SEPCHAIN_API
 * @brief           Core management functions for hash table with separate chaining.
 * @param ht        A Pointer to the hash table instance.
 * @param key       The key used for identification/ordering.
 * @{
 */


/**
 * @param m         Capacity of the hash table.
 * @param hasher    Pointer to a custom hash function.
 * @param cmp       Pointer to a custom compare function.
 * @return          Pointer to an empty hash table, or NULL on failure.
 */
rob_ht_sepchain_t* rob_ht_sepchain_create(size_t m, rob_hasher_t hasher, rob_comparator_t cmp);

void rob_ht_sepchain_destroy(rob_ht_sepchain_t **ht);

/**
 * @param value     Value to insert
 * @param override  Boolean flag: if true and the key already exists, it updates the value.
 */
const void* rob_ht_sepchain_put(rob_ht_sepchain_t *ht, void *key, void *value, const int override);

void rob_ht_sepchain_remove(rob_ht_sepchain_t *ht, void *key);

/**
 * @return The value corresponding to the key, if any.
 */
const void* rob_ht_sepchain_search(const rob_ht_sepchain_t *ht, const void *key);

/**
 * @return The load factor of the hash table.
 */
const float rob_ht_sepchain_loadfactor(const rob_ht_sepchain_t *ht);

void rob_ht_sepchain_print(const rob_ht_sepchain_t *ht);

/** @} */


// EXERCISES

const void* rob_ht_sepchain_oput(rob_ht_sepchain_t *ht, void *key, void *value, const int override);

void rob_ht_sepchain_oremove(rob_ht_sepchain_t *ht, void *key);

const void** rob_ht_sepchain_get_keys(const rob_ht_sepchain_t *ht);


#endif