#include <rob/types/hashtable/sepchain.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROB_HT_SEPCHAIN_SIZE            sizeof(rob_ht_sepchain_t)
#define ROB_HT_SEPCHAIN_SLOT_SIZE       sizeof(rob_ht_sepchain_slot_t)
#define ROB_HT_SEPCHAIN_NODE_SIZE       sizeof(rob_ht_sepchain_node_t)


typedef struct rob_ht_sepchain_node_s rob_ht_sepchain_node_t;
typedef struct rob_ht_sepchain_slot_s rob_ht_sepchain_slot_t;


struct rob_ht_sepchain_node_s { void *key, *value; rob_ht_sepchain_node_t *next; };
struct rob_ht_sepchain_slot_s { rob_ht_sepchain_node_t *head; };


struct rob_ht_sepchain_s {

    size_t m, n;

    rob_ht_sepchain_slot_t *slots;

    rob_hasher_t hasher;

    rob_comparator_t cmp;
};


static rob_ht_sepchain_node_t* rob_ht_sepchain_node_create(void *key, void *value);


rob_ht_sepchain_t* rob_ht_sepchain_create(size_t m, rob_hasher_t hasher, rob_comparator_t cmp) {

    if (m < 1 || !hasher || !cmp) return NULL;

    rob_ht_sepchain_t *ht = malloc(ROB_HT_SEPCHAIN_SIZE);

    if (!ht) return NULL;

    ht->n = 0;
    ht->m = m;
    ht->hasher = hasher;
    ht->cmp = cmp;
    ht->slots = calloc(m, ROB_HT_SEPCHAIN_SLOT_SIZE);

    return ht;
}

void rob_ht_sepchain_destroy(rob_ht_sepchain_t **ht) {

    if (!ht || !*ht) return;

    for (size_t i = 0; i < (*ht)->m; i++) {

        rob_ht_sepchain_slot_t *slot = &(*ht)->slots[i];

        rob_ht_sepchain_node_t *current = slot->head;

        while (current) {

            rob_ht_sepchain_node_t *next = current->next;

            free(current);

            current = next;
        }
    }
    
    free((*ht)->slots);
    free(*ht);

    *ht = NULL;
}


const void* rob_ht_sepchain_put(rob_ht_sepchain_t *ht, void *key, void* value, const int override) {

    if (!ht || !key) return NULL;

    const size_t index = ht->hasher(key, ht->m);

    rob_ht_sepchain_node_t **current = &ht->slots[index].head;

    void *old_value = NULL;

    while (*current) {

        rob_ht_sepchain_node_t *target = *current;

        if (ht->cmp(key, target->key) == 0) {

            if (override) {
                old_value = target->value;
                target->value = value;
            }

            return old_value;
        }

        current = &target->next;
    }

    rob_ht_sepchain_node_t *node = rob_ht_sepchain_node_create(key, value);

    if (!node) return NULL;

    node->next = *current;
    *current = node;

    ht->n++;

    return NULL;
}

void rob_ht_sepchain_remove(rob_ht_sepchain_t *ht, void *key) {

    if (!ht || !key) return;

    const size_t index = ht->hasher(key, ht->m);

    rob_ht_sepchain_node_t **current = &ht->slots[index].head;

    while (*current) {

        rob_ht_sepchain_node_t *target = *current;

        if (ht->cmp(key, (*current)->key) == 0) {

            *current = target->next;

            free(target);

            ht->n--;

            return;
        }

        current = &target->next;
    }
}

const void* rob_ht_sepchain_search(const rob_ht_sepchain_t *ht, const void* key) {

    if (!ht || !key) return NULL;

    const size_t index = ht->hasher(key, ht->m);

    rob_ht_sepchain_slot_t *slot = &ht->slots[index];

    rob_ht_sepchain_node_t *current = slot->head;

    while (current) {

        if (ht->cmp(key, current->key) == 0) return current->value;

        current = current->next;
    }

    return NULL;
}

const float rob_ht_sepchain_loadfactor(const rob_ht_sepchain_t *ht) {

    if (!ht) return -1;

    return (float) ht->n / ht->m;
}

void rob_ht_sepchain_print(const rob_ht_sepchain_t *ht) {

    if (!ht) return;

    printf("Capacity: %lu\n", ht->m);
    printf("Load factor: %0.2f\n", rob_ht_sepchain_loadfactor(ht));

    for (size_t i = 0; i < ht->m; i++) {

        rob_ht_sepchain_slot_t *slot = &ht->slots[i];

        rob_ht_sepchain_node_t *current = slot->head;

        printf("%2lu:  ", i);

        while (current) {

            printf("%d -> ", *(int*) current->key);

            current = current->next;
        }

        printf("\n");
    }
}

static rob_ht_sepchain_node_t* rob_ht_sepchain_node_create(void *key, void *value) {

    rob_ht_sepchain_node_t* node = malloc(ROB_HT_SEPCHAIN_NODE_SIZE);

    if (!node) return NULL;
    
    node->key = key;    
    node->value = value;
    node->next = NULL;

    return node;
}

const void* rob_ht_sepchain_oput(rob_ht_sepchain_t *ht, void *key, void *value, const int override) {

    if (!ht || !key) return NULL;

    const size_t index = ht->hasher(key, ht->m);

    rob_ht_sepchain_node_t **current = &ht->slots[index].head;

    void* old_value = NULL;

    while (*current) {

        rob_ht_sepchain_node_t *target = *current;

        const int res = ht->cmp(key, target->key);

        if (res < 0) break;

        if (res == 0) {
            
            if (override) {
                old_value = target->value;
                target->value = value;
            }

            return old_value;
        }

        current = &target->next;
    }

    rob_ht_sepchain_node_t *node = rob_ht_sepchain_node_create(key, value);
    
    if (!node) return NULL;

    node->next = *current;
    *current = node;

    ht->n++;

    return NULL;
}

void rob_ht_sepchain_oremove(rob_ht_sepchain_t *ht, void *key) {

    if (!ht || !key) return;

    const size_t index = ht->hasher(key, ht->m);

    rob_ht_sepchain_node_t **current = &ht->slots[index].head;

    while (*current) {

        rob_ht_sepchain_node_t *target = *current;

        const int res = ht->cmp(key, target->key);

        if (res < 0) return;

        if (res == 0) {

            *current = target->next;

            free(target);

            ht->n--;

            return;
        }

        current = &target->next;
    }
}

const void** rob_ht_sepchain_get_keys(const rob_ht_sepchain_t *ht) {

    if (!ht || ht->n == 0) return NULL;

    const void **keys = malloc(ht->n * sizeof ht->slots[0].head->key);

    size_t j = 0;

    for (size_t i = 0; i < ht->m; i++) {

        rob_ht_sepchain_node_t *current = ht->slots[i].head;

        while (current) {
            
            keys[j++] = current->key;

            current = current->next;
        }
    }

    return keys;
}