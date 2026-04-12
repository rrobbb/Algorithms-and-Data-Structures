#include <rob/types/bst.h>

#include <stdio.h>

static void test_rob_bst();

int main(void) {

    test_rob_bst();

    return 0;
} 

static void test_rob_bst() {

    rob_bst_t *tree = rob_bst_create(rob_int_cmp);

    int keys[] = {4, 8, 3, 7, 11};

    size_t n = sizeof keys / sizeof keys[0];
    
    for (size_t i = 0; i < n; i++) rob_bst_put(tree, &keys[i], NULL, 0);

    rob_bst_print(tree);

    const int *successor = rob_bst_successor(tree, &keys[0]);

    printf("Successor: %d\n", *successor);

    rob_bst_delete(&tree, 0);
}