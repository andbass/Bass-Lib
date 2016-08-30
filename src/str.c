
#define _GNU_SOURCE

#include "bass/str.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdarg.h>

BStrInternal* BStr_fit_size(BStr str, u64 size) {
    BStrInternal* str_int = BStr_internal(str);
    if ((size + 1) > str_int->cap) { // Add one for the null term
        u64 new_cap = (size + 1) * 2;

        str_int = realloc(str_int, sizeof(BStrInternal) + new_cap);
        str_int->cap = new_cap;
    }

    return str_int;
}

BStr BStr_new(char* src) {
    u64 src_size = strlen(src) + 1;
    u64 initial_cap = src_size * 2;

    BStrInternal* str_internal = malloc(sizeof(BStrInternal) + initial_cap);
    *str_internal = (BStrInternal){
        .len = src_size - 1,
        .cap = initial_cap,
    };

    strcpy(str_internal->buf, src);
    return (BStr)&str_internal->buf;
}

void BStr_clean(BStr* str) {
    if (*str == NULL) {
        return;
    }

    BStrInternal* str_int = BStr_internal(*str);
    free(str_int);

    *str = NULL;
}

u64 BStr_len(BStr str) {
    BStrInternal* str_int = BStr_internal(str);
    return str_int->len;
}

BStr BStr_empty() {
    return BStr_with_cap(64);
}

BStr BStr_with_cap(u64 cap) {
    BStrInternal* str_internal = malloc(sizeof(BStrInternal) + cap);
    *str_internal = (BStrInternal){
        .len = 0,
        .cap = cap,
    };

    str_internal->buf[0] = '\0';
    return (BStr)&str_internal->buf;
}

// TODO not use "vasprintf"
BStr BStr_format(char* fmt, ...) {
    va_list list;
    va_start(list, fmt);

    char* cstr;
    int error = vasprintf(&cstr, fmt, list);

    if (error == -1) {
        return NULL;
    }

    va_end(list);
    BStr str = BStr_new(cstr);

    free(cstr);
    return str;
}

// TODO make more efficent
BStr BStr_concat(BStr str1, BStr str2) {
    BStr result = BStr_empty();

    BStr_push_str(&result, str1);
    BStr_push_str(&result, str2);

    return result;
}

BStr BStr_cconcat(char* str1, char* str2) {
    BStr result = BStr_empty();

    BStr_push_cstr(&result, str1);
    BStr_push_cstr(&result, str2);

    return result;
}

bool BStr_eq(BStr str1, BStr str2) {
    u64 len = BStr_len(str1);
    if (len != BStr_len(str2)) {
        return false;
    }

    for (u64 i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }

    return true;
}

// TODO remove code duplication
bool BStr_ceq(BStr str1, char* str2) {
    u64 len = BStr_len(str1);
    if (len != strlen(str2)) {
        return false;
    }

    for (u64 i = 0; i < len; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }

    return true;
}

BStr BStr_sub(BStr str, u64 start, u64 end) {
    if (start >= BStr_len(str) || end > BStr_len(str)) {
        return NULL;
    }

    u64 len = end - start;

    BStr slice = BStr_with_cap(len * 2);
    BStrInternal* slice_int = BStr_internal(slice);

    memcpy(slice_int->buf, &str[start], len);
    slice[len] = '\0';

    return slice;
}

BStr BStr_sub_end(BStr str, u64 start) {
    return BStr_sub(str, start, BStr_len(str));
}

void BStr_push(BStr* str, char ch) {
    u64 new_len = BStr_len(*str) + 1;
    BStrInternal* str_int = BStr_fit_size(*str, new_len);

    str_int->buf[str_int->len] = ch;
    str_int->buf[str_int->len + 1] = '\0';

    str_int->len = new_len;
    *str = str_int->buf;
}

void BStr_push_str(BStr* to, BStr from) {
    u64 new_len = BStr_len(*to) + BStr_len(from);
    BStrInternal* str_int = BStr_fit_size(*to, new_len);

    memcpy(&str_int->buf[str_int->len], from, BStr_len(from));
    str_int->buf[new_len] = '\0';

    str_int->len = new_len;
    *to = str_int->buf;
}

void BStr_push_cstr(BStr* to, char* from) {
    u64 from_len = strlen(from);
    u64 new_len = BStr_len(*to) + strlen(from);

    BStrInternal* str_int = BStr_fit_size(*to, new_len);

    memcpy(&str_int->buf[str_int->len], from, from_len);
    str_int->buf[new_len] = '\0';

    str_int->len = new_len;
    *to = str_int->buf;
}

void BStr_shrink(BStr* str) {
    BStrInternal* str_int = BStr_internal(*str);

    str_int = realloc(str_int, sizeof(BStrInternal) + str_int->len + 1);
    str_int->cap = str_int->len + 1;

    *str = str_int->buf;
}

BStrInternal* BStr_internal(BStr str) {
    return (BStrInternal*)((u8*)str - offsetof(BStrInternal, buf));
}

