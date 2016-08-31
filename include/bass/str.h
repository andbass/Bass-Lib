#pragma once
/*
 * A heap-allocated dynamic string struct inspired by antirez's excellent SDS library: https://github.com/antirez/sds
 */

#include "bass/types.h"

#include <stdbool.h>

typedef struct {
    u64 len;
    u64 cap;

    char buf[0];
} BStrInternal;

typedef char* BStr;

// "src" is a valid, null-terminated C string
BStr BStr_new(char* src);
void BStr_clean(BStr*);

u64 BStr_len(BStr);

// Creates an empty string with a reasonable large capacity (64 bytes)
BStr BStr_empty();
BStr BStr_with_cap(u64 cap);

BStr BStr_format(char* fmt, ...);

// Creates a new string by adding together two strings
BStr BStr_concat(BStr, BStr);
BStr BStr_cconcat(char*, char*);

bool BStr_eq(BStr, BStr);
bool BStr_ceq(BStr, char*);

// Substring
BStr BStr_sub(BStr, i64 start, i64 end);
BStr BStr_sub_end(BStr, i64 start); // Substrings to end of string

void BStr_push(BStr*, char);
void BStr_push_str(BStr*, BStr);
void BStr_push_cstr(BStr*, char*);

// Changes "cap" to "len", will require reallocation if anything is added afterwards
void BStr_shrink(BStr*);

// Acceses internal data about a "BStr"
BStrInternal* BStr_internal(BStr);

// Ensures "str" fits "size", which may return a new string if reallocation occurs
BStrInternal* BStr_fit_size(BStr str, u64 size);

