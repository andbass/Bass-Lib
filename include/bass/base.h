#pragma once
/*
 * Base macros and functions used by just about everything
 */

// Automatically cleans up data-structures when they go out of scope
#define autodel(type) \
    __attribute__((__cleanup__(type ## _clean))) type 
