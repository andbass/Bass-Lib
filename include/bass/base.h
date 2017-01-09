#pragma once
/*
 * Base macros and functions used by just about everything
 */

// Automatically cleans up data-structures when they go out of scope
#define AUTODEL(type) \
    __attribute__((__cleanup__(type ## _clean))) type 

// Creates an anonymous function
#define LAMBDA(ret_type, args_with_body) ({ \
    ret_type _function args_with_body \
    _function; \
})

#define AUTO __auto_type

#define MAKE(type, ...) ({ \
    type tmp; \
    type ## _init(&tmp, __VA_ARGS__); \
    tmp; \
})
