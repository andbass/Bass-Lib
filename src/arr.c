
#include "bass/arr.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void BArr_resize(BArr* arr) {
    i64 new_cap = arr->cap << 1;

    arr->buf = realloc(arr->buf, new_cap * arr->elem_size);
    arr->cap = new_cap;
}

static i64 BArr_offset(BArr* arr, i64 index) {
    return index * arr->elem_size;
}

void BArr_init(BArr* arr, size_t elem_size) {
    arr->len = 0;
    arr->elem_size = elem_size;

    const i64 target_cap = 128;

    arr->buf = malloc(elem_size * target_cap);
    arr->cap = target_cap;
}

void BArr_clean(BArr* arr) {
    free(arr->buf);
    arr->buf = NULL;

    arr->len = 0;
    arr->cap = 0;
}

void BArr_push(BArr* arr, void* data) {
    if (arr->len == arr->cap) {
        BArr_resize(arr);
    }

    i64 offset = BArr_offset(arr, arr->len);
    memcpy(&arr->buf[offset], data, arr->elem_size);

    arr->len++;
}

void* BArr_get(BArr* arr, i64 index) {
    if (index >= arr->len) {
        return NULL;
    }

    i64 offset = BArr_offset(arr, index);
    return &arr->buf[offset];
}

void BArr_remove(BArr* arr, i64 index) {
    assert(index < arr->len);
    i64 next_index = index + 1;

    i64 offset = BArr_offset(arr, index);
    i64 next_offset = BArr_offset(arr, next_index);

    memmove(&arr->buf[offset], &arr->buf[next_offset], BArr_used_size(arr) - next_offset);
}

void BArr_clear(BArr* arr) {
    BArr_clean(arr);
    BArr_init(arr, arr->elem_size);
}

size_t BArr_used_size(BArr* arr) {
    return arr->len * arr->elem_size;
}
