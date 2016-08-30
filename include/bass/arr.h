#pragma once
/*
 * Dynamically sized array that stores its contains continuously on the heap
 * It lacks type safety (but works with any type of data structure), so be careful
 */

#include "bass/types.h"

#include <stdio.h>
#include <stdbool.h>

typedef struct {
    u8* buf;
    size_t elem_size;

    i64 len;
    i64 cap;
} BArr;

void BArr_init(BArr*, size_t elem_size);
void BArr_clean(BArr*);

void BArr_push(BArr*, void* data);
void BArr_pop(BArr*, void* dest);

void* BArr_get(BArr*, u64 index);
void BArr_remove(BArr*, u64 index);

void BArr_clear(BArr*);

size_t BArr_used_size(BArr*);

// Utility macros
#define MAKE_ARR(type) ({ \
    BArr tmp; \
    BArr_init(&tmp, sizeof(type)); \
    tmp; \
})

#define FOR_EACH_ARR(index, type, elem, arr) \
    i32 index = -1; \
    type elem = NULL; \
    while ((++index, elem = BArr_get(&(arr), index)) != NULL)

// For pushing on literal values (also supports named variables
#define ARR_PUSH(arr, lit) { \
    __typeof__(lit) tmp = (lit); \
    BArr_push(&(arr), &tmp); \
}
