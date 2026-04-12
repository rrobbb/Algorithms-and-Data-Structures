#include <rob/types/hashtable/linprob.h>
#include <rob/types/hashtable/sepchain.h>

#include <stddef.h>
#include <stdio.h>


static void test_linprob();

static void test_sepchain();


static int keys[4] = {0, 8, 16, 32};

static const size_t n = sizeof keys / sizeof keys[0];


int main(void) {

    test_sepchain();
    
    return 0;
}


static void test_linprob() {

    rob_ht_linprob_t *ht = rob_ht_linprob_create(8, rob_int_hash, rob_int_cmp);

    for (size_t i = 0; i < n; i++) 
        rob_ht_linprob_put(ht, &keys[i], NULL, 0);

    rob_ht_linprob_print(ht);

    rob_ht_linprob_remove(ht, &keys[0]);
    rob_ht_linprob_remove(ht, &keys[1]);
    rob_ht_linprob_remove(ht, &keys[2]);

    rob_ht_linprob_print(ht);

    rob_ht_linprob_delete(&ht, 0);
}

static void test_sepchain() {

    int keys[4] = {32,16, 8, 0};

    const size_t n = sizeof keys / sizeof keys[0];

    rob_ht_sepchain_t *ht = rob_ht_sepchain_create(8, rob_int_hash, rob_int_cmp);

    for (size_t i = 0; i < n; i++) 
        rob_ht_sepchain_oput(ht, &keys[i], NULL, 0);

    rob_ht_sepchain_print(ht);

    rob_ht_sepchain_oremove(ht, &keys[0]);
    rob_ht_sepchain_oremove(ht, &keys[1]);
    rob_ht_sepchain_oremove(ht, &keys[2]);

    rob_ht_sepchain_print(ht);

    rob_ht_sepchain_delete(&ht);
}