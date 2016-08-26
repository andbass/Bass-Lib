
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define TEST(name) bool test_ ## name()

#define PASS return true
#define FAIL(msg) printf("\033[1m\033[31mFAIL \033[37m[%s:%d]\033[0m %s\n", __FILE__, __LINE__, msg); \
    return false

#define EQ(x, y, msg) if (x != y) { FAIL(msg); }
#define NOT_EQ(x, y, msg) if (x == y) { FAIL(msg); }

#define STR_EQ(x, y, msg) if (strcmp(x, y) != 0) { FAIL(msg); }
#define NOT_STR_EQ(x, y, msg) if (strcmp(x, y) == 0) { FAIL(msg); }

#define GT(x, y, msg) if (x =< y) { FAIL(msg); }
#define LT(x, y, msg) if (x >= y) { FAIL(msg); }

#define GT_EQ(x, y, msg) if (x < y) { FAIL(msg); }
#define LT_EQ(x, y, msg) if (x > y) { FAIL(msg); }

#define NOT_NULL(x, msg) if (x == NULL) { FAIL(msg); }
