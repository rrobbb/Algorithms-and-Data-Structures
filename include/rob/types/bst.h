#ifndef ROB_BST_H
#define ROB_BST_H

#include <rob/utils.h>


struct rob_bst_s;

typedef struct rob_bst_s rob_bst_t;


/**
 * @defgroup            BST_API Binary Search Tree Core API
 * @brief               Core management functions for the Binary Search Tree.
 * @param tree          Pointer to the BST instance.
 * @param key           The key used for identification/ordering.
 * @param destroy_data  Boolean flag: if true, internal data is deallocated.
 * @{
 */

/**
 * @param   cmp 
 * @return  A pointer to an empty BST, or NULL on failure
 */
rob_bst_t* rob_bst_create(rob_comparator_t cmp);

void rob_bst_destroy(rob_bst_t **tree, const int destroy_data);

/**
 * @param value     Value associated with the key.
 * @param override  Boolean flag: if true, replaces the value of an existing key.
 * @return          The old value if overridden, otherwise NULL.
 */
const void* rob_bst_put(rob_bst_t *tree, void* key, void* value, const int override);

/**
 * @brief Removes a node from the tree.
 */
void rob_bst_remove(rob_bst_t *tree, void* key, const int destroy_data);

/**
 * @return The value corresponding to the key, if any.
 */
const void* rob_bst_search(const rob_bst_t *tree, const void* key);

int rob_bst_height(const rob_bst_t *tree, const void* key);

/**
 * @param print Custom print function.
 */
void rob_bst_print_in_order(const rob_bst_t *tree, rob_printer_t print);

/** @} */


// EXERCISES

/**
 * @return The predecessor (the key before) of the given key.
 */
const void* rob_bst_predecessor(const rob_bst_t *tree, const void *key);

/**
 * @return The successor (the key immediatly after) of the given key.
 */
const void* rob_bst_successor(const rob_bst_t *tree, const void *key);

size_t rob_bst_subtree_count_even(const rob_bst_t *tree, const void *key);

const int rob_bst_balancefactor(const rob_bst_t *tree, const void* key);


#endif