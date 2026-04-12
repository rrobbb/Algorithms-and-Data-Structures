#include <rob/types/hashtable/linprob.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROB_HT_LINPROB_SIZE         sizeof(rob_ht_linprob_t)
#define ROB_HT_LINPROB_SLOT_SIZE    sizeof(rob_ht_linprob_slot_t)


struct rob_ht_linprob_slot_s;

typedef struct rob_ht_linprob_slot_s rob_ht_linprob_slot_t;


struct rob_ht_linprob_slot_s { void *key, *value; int tombstone; };


struct rob_ht_linprob_s {

    size_t m, n;

    rob_ht_linprob_slot_t *slots;

    rob_hasher_t hasher;

    rob_comparator_t cmp;
};


static void rob_ht_slot_insert(rob_ht_linprob_slot_t *slot, void *key, void *value);

static void rob_ht_slot_remove(rob_ht_linprob_slot_t *slot);

static void rob_ht_resize(rob_ht_linprob_t *ht, size_t m);


rob_ht_linprob_t* rob_ht_linprob_create(size_t m, rob_hasher_t hasher, rob_comparator_t cmp) {

    if (m < 2 || !hasher || !cmp) return NULL;

    rob_ht_linprob_t *ht = malloc(ROB_HT_LINPROB_SIZE);

    if (!ht) return NULL;

    ht->n = 0;
    ht->m = m;
    ht->hasher = hasher;
    ht->cmp = cmp;
    ht->slots = calloc(m, ROB_HT_LINPROB_SLOT_SIZE);

    return ht;
}

void rob_ht_linprob_destroy(rob_ht_linprob_t **ht, int destroy_data) {

    if (!ht || !*ht) return;

    for (size_t i = 0; i < (*ht)->m; i++)
        if ((*ht)->slots[i].key != NULL) {
            if (destroy_data) {
                free((*ht)->slots[i].key);
                free((*ht)->slots[i].value);
            }
        }
    
    free((*ht)->slots);
    free(*ht);

    *ht = NULL;
}


void rob_ht_linprob_put(rob_ht_linprob_t *ht, void *key, void* value, int override) {

    if (!ht || !key) return;

    if (rob_ht_linprob_loadfactor(ht) >= 0.75) rob_ht_resize(ht, ht->m * 2);

    size_t index = ht->hasher(key, ht->m);

    int first_tombstone = -1;

    for (size_t i = 0; i < ht->m; i++) {

        rob_ht_linprob_slot_t *slot = &ht->slots[index];

        if (slot->tombstone == 0) {

            if (!slot->key) break;

            if (ht->cmp(key, slot->key) == 0) {

                if (override) slot->value = value;
                
                return;
            }

        } else if (first_tombstone == -1) first_tombstone = (int) index;

        index = (index + 1) % ht->m;
    }

    index = (first_tombstone != -1) ? first_tombstone : index;

    rob_ht_slot_insert(&ht->slots[index], key, value);
        
    ht->n++;
}

void rob_ht_linprob_remove(rob_ht_linprob_t *ht, void *key) {

    if (!ht || !key) return;

    size_t index = ht->hasher(key, ht->m);

    for (size_t i = 0; i < ht->m; i++) {

        rob_ht_linprob_slot_t *slot = &ht->slots[index];

        if (slot->tombstone == 0) {

            if (!slot->key) return;

            if (ht->cmp(key, slot->key) == 0) {

                rob_ht_slot_remove(slot);

                ht->n--;

                return;
            }

        }

        index = (index + 1) % ht->m;
    }

    if (rob_ht_linprob_loadfactor(ht) <= 0.125) rob_ht_resize(ht, ht->m / 2);
}

const void* rob_ht_search(const rob_ht_linprob_t *ht, const void* key) {

    if (!ht || !key) return NULL;

    size_t index = ht->hasher(key, ht->m);

    for (size_t i = 0; i < ht->m; i++) {

        rob_ht_linprob_slot_t *slot = &ht->slots[index];

        if (slot->tombstone == 0 && !slot->key) return NULL;

        if (slot->tombstone == 0 && ht->cmp(key, slot->key) == 0) return slot->value;
        
        index = (index + 1) % ht->m;
    }

    return NULL;
}

const float rob_ht_linprob_loadfactor(const rob_ht_linprob_t *ht) {

    if (!ht) return -1;

    return (float) ht->n / ht->m;
}

void rob_ht_linprob_print(const rob_ht_linprob_t *ht, rob_printer_t print) {

    if (!ht) return;

    printf("Capacity: %lu\n", ht->m);
    printf("Load factor: %0.2f\n", rob_ht_linprob_loadfactor(ht));

    for (size_t i = 0; i < ht->m; i++) {

        rob_ht_linprob_slot_t* slot = &ht->slots[i];

        printf("%2lu | ", i);
        
        if (slot->tombstone) printf("TS\n");
        
        else if (slot->key) print(slot->key, slot->value);
        
        else printf("EMPTY\n");
    }
}

void rob_ht_merge(rob_ht_linprob_t *dest, const rob_ht_linprob_t *src) {

    if (!dest || !src) return;

    if (dest->hasher != src->hasher || dest->cmp != src->cmp) return;

    for (size_t i = 0; i < src->m; i++) {

        rob_ht_linprob_slot_t *slot = &src->slots[i];

        if (slot->key && slot->tombstone == 0) 
            rob_ht_linprob_put(dest, slot->key, slot->value, 0);
    }
}

void rob_ht_difference(rob_ht_linprob_t *dest, const rob_ht_linprob_t *src) {

    if (!dest || !src) return;

    if (dest->hasher != src->hasher || dest->cmp != src->cmp) return;

    for (size_t i = 0; i < src->m; i++) {

        rob_ht_linprob_slot_t *slot = &src->slots[i];

        if (slot->key && slot->tombstone == 0) 
            rob_ht_linprob_remove(dest, slot->key);
    }
}

static void rob_ht_resize(rob_ht_linprob_t *ht, size_t m) {

    const size_t old_m = ht->m;

    rob_ht_linprob_slot_t *old_slots = ht->slots;

    ht->m = m;
    ht->n = 0;
    ht->slots = calloc(ht->m, ROB_HT_LINPROB_SLOT_SIZE);

    for (size_t i = 0; i < old_m; i++) {

        rob_ht_linprob_slot_t *old_slot = &old_slots[i];

        if (old_slot->key && old_slot->tombstone == 0) {
            
            size_t index = ht->hasher(old_slot->key, ht->m);
            
            while (ht->slots[index].key) index = (index + 1) % ht->m;

            rob_ht_slot_insert(&ht->slots[index], old_slot->key, old_slot->value);

            ht->n++;
        }
    }

    free(old_slots);
}

static void rob_ht_slot_insert(rob_ht_linprob_slot_t *slot, void *key, void *value) {
    slot->key = key;
    slot->value = value;
    slot->tombstone = 0;
}

static void rob_ht_slot_remove(rob_ht_linprob_slot_t *slot) {
    slot->key = NULL;
    slot->value = NULL;
    slot->tombstone = 1;
}