
#include <rob/types/bst.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define ROB_BST_SIZE        sizeof(rob_bst_t)
#define ROB_BST_NODE_SIZE   sizeof(rob_bst_node_t)


/* STRUCTS AND TYPES DEFINITIONS */

struct rob_bst_node_s;

typedef struct rob_bst_node_s rob_bst_node_t;


/* STRUCTS AND TYPES IMPLEMENTATIONS */

struct rob_bst_node_s { void *key, *value; rob_bst_node_t *right, *left; };

struct rob_bst_s { rob_bst_node_t *root; rob_comparator_t cmp; };


/* STATIC FUNCTIONS DECLARATION */

static rob_bst_node_t* rob_bst_node_create(void *key, void *value);

static void rob_bst_node_delete(rob_bst_node_t **node, const int destroy_data);

static void rob_bst_node_delete_all(rob_bst_node_t **node, const int destroy_data);

static const void* rob_bst_node_update(rob_bst_node_t *node, void *value);

static int rob_bst_node_height(const rob_bst_node_t *node);

static void rob_bst_node_print_in_order(const rob_bst_node_t *node, rob_printer_t print);

static size_t rob_bst_node_subtree_count_leaves_depth(rob_bst_node_t *node, size_t depth, size_t current_depth);

static size_t rob_bst_rank_impl(const rob_bst_node_t *node, const void *key, rob_comparator_t cmp);


/* MAIN METHODS */

rob_bst_t* rob_bst_create(rob_comparator_t cmp) {

    rob_bst_t *tree = malloc(ROB_BST_SIZE);

    tree->root = NULL, tree->cmp = cmp;

    return tree;
}

void rob_bst_destroy(rob_bst_t **tree, const int destroy_data) {

    if (!tree || !*tree) return;

    if ((*tree)->root) rob_bst_node_delete_all(&(*tree)->root, destroy_data);

    free(*tree);

    *tree = NULL;
}

const void* rob_bst_put(rob_bst_t *tree, void* key, void* value, const int override) {

    if (!tree) return NULL;

    rob_bst_node_t **current = &tree->root;

    while (*current) {

        rob_bst_node_t *target = *current;

        const int res = tree->cmp(key, target->key);

        if (res == 0) return (override) ? rob_bst_node_update(target, value) : NULL;
        
        current = (res > 0) ? &target->right : &target->left;
    }

    *current = rob_bst_node_create(key, value);

    return NULL;
}

void rob_bst_remove(rob_bst_t *tree, void* key, const int destroy_data) {

    if (!tree || !tree->root || !key) return;

    rob_bst_node_t **current = &tree->root;

    while (*current) {

        rob_bst_node_t *target = *current;

        const int res = tree->cmp(key, target->key);

        if (res == 0) {

            if (target->left && target->right) {

                rob_bst_node_t **predecessor = &target->left;

                while ((*predecessor)->right) predecessor = &(*predecessor)->right;

                target->key = (*predecessor)->key, target->value = (*predecessor)->value;

                target = *predecessor;

                *predecessor = target->left;

                target->key = NULL;
                target->value = NULL;

            } else *current = (target->left) ? target->left : target->right;
            
            rob_bst_node_delete(&target, destroy_data);

            return;
        }

        current = (res > 0) ? &target->right : &target->left;
    }
}

const void* rob_bst_search(const rob_bst_t *tree, const void* key) {

    if (!tree || !key) return NULL;

    rob_bst_node_t *current = tree->root;

    while (current) {

        const int res = tree->cmp(key, current->key);

        if (res == 0) return current->value;

        current = (res > 0) ? current->right : current->left;
    }

    return NULL;
}

void rob_bst_print_in_order(const rob_bst_t *tree, rob_printer_t print) {

    if (!tree || !tree->root) return;

    rob_bst_node_print_in_order(tree->root, rob_print_int_string);
}


/* EXERCISES */

const int rob_bst_balancefactor(const rob_bst_t *tree, const void* key) {

    if (!tree || !key) return -255;

    rob_bst_node_t *current = tree->root;

    while (current) {

        const int res = tree->cmp(key, current->key);

        if (res == 0) return rob_bst_node_height(current->left) - rob_bst_node_height(current->right);

        current = (res > 0) ? current->right : current->left;
    }

    return -255;
}

const void* rob_bst_predecessor(const rob_bst_t *tree, const void* key) {

    if (!tree || !key) return NULL;

    rob_bst_node_t *current = tree->root, *predecessor = NULL;

    while (current) {

        const int res = tree->cmp(key, current->key);

        if (res == 0) {

            if (current->left) {

                predecessor = current->left;

                while (predecessor->right) predecessor = predecessor->right;
            }

            return (predecessor) ? predecessor->key : NULL;
        }

        if (res > 0) { predecessor = current; current = current->right; } else current = current->left;
    }

    return NULL;
}

const void* rob_bst_successor(const rob_bst_t *tree, const void* key) {

    if (!tree || !key) return NULL;

    rob_bst_node_t *current = tree->root, *successor = NULL;

    while (current) {

        const int res = tree->cmp(key, current->key);

        if (res == 0) {

            if (current->right) {

                successor = current->right;

                while (successor->left) successor = successor->left;
            }

            return (successor) ? successor->key : NULL;
        }

        if (res > 0) current = current->right; else { successor = current; current = current->left; }
    }

    return NULL;
}

size_t rob_bst_subtree_count_leaves_depth(const rob_bst_t *tree, const void *key, size_t depth) {

    if (!tree || !key) return 0;

    rob_bst_node_t *current = tree->root;

    size_t current_depth = 0;

    while (current) {

        const int res = tree->cmp(key, current->key);

        if (res == 0) return rob_bst_node_subtree_count_leaves_depth(current, depth, current_depth);

        current = (res > 0) ? current->right : current->left;

        current_depth++;
    }

    return 0;
}

static size_t rob_bst_node_subtree_count_leaves_depth(rob_bst_node_t *node, size_t depth, size_t current_depth) {

    if (!node || current_depth > depth) return 0;

    if (current_depth == depth) return (!node->left && !node->right) ? 1 : 0;

    return rob_bst_node_subtree_count_leaves_depth(node->left, depth, current_depth + 1) + rob_bst_node_subtree_count_leaves_depth(node->right, depth, current_depth + 1);
}

void* rob_bst_get_value_depth(const rob_bst_t *tree, const void *key, long *depth) {

    if (!tree || !key || !depth) return NULL;
    
    rob_bst_node_t *current = tree->root;

    *depth = 0;

    while (current) {

        const int res = tree->cmp(key, current->key);

        if (res == 0) return current->value;

        current = (res > 0) ? current->right : current->left;

        *depth = *depth + 1;
    }

    *depth = -1;

    return NULL;
}

size_t rob_bst_rank(const rob_bst_t *tree, const void *key) {

    if (!tree || !key) return 0;

    return rob_bst_rank_impl(tree->root, key, tree->cmp);
}

static size_t rob_bst_rank_impl(const rob_bst_node_t *node, const void *key, rob_comparator_t cmp) {

    if (!node || !key || !cmp) return 0;

    if (cmp(key, node->key) <= 0) return rob_bst_rank_impl(node->left, key, cmp);

    return 1 + rob_bst_rank_impl(node->left, key, cmp) + rob_bst_rank_impl(node->right, key, cmp);
}


/* STATIC FUNCTIONS IMPLEMENTATIONS */

static rob_bst_node_t* rob_bst_node_create(void *key, void *value) {

    rob_bst_node_t *node = malloc(ROB_BST_NODE_SIZE);

    if (!node) return NULL;

    node->key = key, node->value = value;
    node->left = node->right = NULL;

    return node;
}

static void rob_bst_node_delete(rob_bst_node_t **node, int destroy_data) {

    if (!node || !*node) return;

    if (destroy_data) {
        free((*node)->key);
        free((*node)->value);
    }

    free(*node);

    *node = NULL;
}

static void rob_bst_node_delete_all(rob_bst_node_t **root, int destroy_data) {

    if (!root || !*root) return;

    rob_bst_node_delete_all(&(*root)->left, destroy_data);
    rob_bst_node_delete_all(&(*root)->right, destroy_data);

    rob_bst_node_delete(root, destroy_data);
}

static const void* rob_bst_node_update(rob_bst_node_t *node, void *value) {

    if (!node) return NULL;

    void* old_value = node->value;

    node->value = value;

    return old_value;
}

static int rob_bst_node_height(const rob_bst_node_t *node) {
    
    if (!node) return -1;

    int left = rob_bst_node_height(node->left);
    int right = rob_bst_node_height(node->right);

    return 1 + (left > right ? left : right);
}

static void rob_bst_node_print_in_order(const rob_bst_node_t *node, rob_printer_t print) {

    if (!node) return;

    rob_bst_node_print_in_order(node->left, print);

    print(node->key, node->value);

    rob_bst_node_print_in_order(node->right, print);
}