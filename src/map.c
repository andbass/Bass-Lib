
#include "bass/map.h"

#include <stdlib.h>
#include <string.h>

static void BMap_clean_pair(BMapPair* pair) {
    if (pair->key != NULL) {
        free(pair->key);

        if (pair->next != NULL) {
            BMap_clean_pair(pair->next);

            free(pair->next);
            pair->next = NULL;
        }
    }
}


static void* BMap_search_pair(BMapPair* pair, char* key) {
    if (strcmp(key, pair->key) == 0) {
        return pair->value;
    } else if (pair->next != NULL) {
        return BMap_search_pair(pair->next, key);
    } else {
        return NULL;
    }
}

BMapPair* BMap_get_index(BMap* map, u64 index) {
    u64 offset = index * map->pair_size;
    u8* raw_pairs = (u8*)map->pairs;

    u8* raw_pair = &raw_pairs[offset];
    return (BMapPair*)raw_pair;
}

BMapPair* BMap_get_pair(BMap* map, char* key) {
    u64 hash = hash_fnv1a(key);
    u64 index = hash & (map->total_pairs - 1);

    return BMap_get_index(map, index);
}

void BMap_init(BMap* map, size_t value_size, u64 size) {
    map->value_size = value_size;
    map->pair_size = sizeof(BMapPair) + value_size;

    map->total_pairs = size;

    map->pairs = malloc(map->pair_size * map->total_pairs);
    for (u64 i = 0; i < map->total_pairs; i++) {
        *BMap_get_index(map, i) = (BMapPair){
            .key = NULL,
            .next = NULL,
        };
    }
}

void BMap_clean(BMap* map) {
    for (u64 i = 0; i < map->total_pairs; i++) {
        BMap_clean_pair(BMap_get_index(map, i));
    }
}

// TODO use custom string
void BMap_put(BMap* map, char* key, void* value) {
    BMapPair* pair = BMap_get_pair(map, key);

    char* dup_key = malloc(strlen(key) + 1);
    strcpy(dup_key, key);

    if (pair->key == NULL) {
        pair->key = dup_key; // TODO use custom string
        pair->next = NULL;

        memcpy(pair->value, value, map->value_size);
        return;
    }

    while (strcmp(pair->key, key) != 0) {
        pair = pair->next;

        if (pair == NULL) {
            pair = malloc(map->pair_size);
            pair->next = NULL;

            break;
        }
    }

    pair->key = dup_key; // TODO use custom string
    memcpy(pair->value, value, map->value_size);
}

void* BMap_get(BMap* map, char* key) {
    BMapPair* pair = BMap_get_pair(map, key);

    if (pair->key == NULL) {
        return NULL;
    } else {
        return BMap_search_pair(pair, key);
    }
}

void BMap_iter(BMap* map, BMapIter func) {
    BMapIterCode code;

    for (int i = 0; i < map->total_pairs; i++) {
        BMapPair* pair = BMap_get_index(map, i);
        if (pair->key != NULL) {
            code = func(pair->key, pair->value);

            while (code != Exit && pair->next != NULL) {
                pair = pair->next;
                func(pair->key, pair->value);
            }

        }
    }
}

u64 hash_fnv1a(char* str) {
    u64 hash = FNV_OFFSET_BASIS;
    for (u64 i = 0; i < strlen(str); i++) {
        hash ^= str[i];
        hash *= FNV_PRIME;
    }

    return hash;
}
