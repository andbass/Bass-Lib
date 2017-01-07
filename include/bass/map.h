#pragma once
/*
 * Unordered hashmap that uses strings for keys
 * Make sure that the size of the map is a power of 2
 */

#include "bass/arr.h"

typedef enum {
    Continue,
    Exit,
} BMapIterCode;

typedef BMapIterCode (*BMapIter)(char* key, void* value);

typedef struct Tag_BMapPair{
    char* key;
    struct Tag_BMapPair* next;

    u8 value[0];
} BMapPair;

typedef struct {
    size_t value_size; // Size of values stored in map
    size_t pair_size; // Size of each pair, including stored values

    BMapPair* pairs;
    u64 total_pairs; // Number of allocated pairs in "pairs" (not including linked up pairs via "next")
} BMap;

void BMap_init(BMap*, size_t value_size, u64 size);
void BMap_clean(BMap*);

void BMap_put(BMap*, char* key, void* value);
void* BMap_get(BMap*, char* key);

void BMap_iter(BMap*, BMapIter);

BMapPair* BMap_get_index(BMap* map, u64 index);

#define FNV_OFFSET_BASIS 14695981039346656037UL
#define FNV_PRIME 1099511628211UL
u64 hash_fnv1a(char* str);

// Utility macros
#define MAKE_MAP(type, size) MAKE(BMap, sizeof(type), (size))
