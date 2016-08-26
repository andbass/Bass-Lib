/*
 * Test file for arrays
 */

#include "test-helper.h"

#include "bass/arr.h"

TEST(array_init) {
    BArr arr = MAKE_ARR(char*);
    (void)arr;

    EQ(1, 1, "1 is not equal to 1, isnt possible");

    PASS;
}

TEST(array_push) {
    BArr arr = MAKE_ARR(char*);

    EQ(arr.len, 0, "Array is not empty");

    char* str = "Hello world!";
    BArr_push(&arr, &str);

    EQ(arr.len, 1, "Array length unchanged");

    PASS;
}

TEST(array_pop) {
    PASS;
}

TEST(array_index) {
    BArr arr = MAKE_ARR(char*);

    char* str = "Hello world!";
    BArr_push(&arr, &str);

    char** stored_str = BArr_get(&arr, 0);

    NOT_NULL(stored_str, "String not transferred into array");
    STR_EQ(*stored_str, str, "Stored string is not equal to original string");
    
    PASS;
}

TEST(array_clear) {
    BArr arr = MAKE_ARR(int);

    for (int i = 0; i < 1000; i++) {
        BArr_push(&arr, &i);
    }

    BArr_clear(&arr);

    EQ(arr.len, 0, "Array's length not reset");
    EQ(arr.cap, 128, "Array's capacity not reset");

    PASS;
}

TEST(array_remove) {
    BArr arr = MAKE_ARR(int);

    for (int i = 0; i < 10; i++) {
        BArr_push(&arr, &i);
    }

    BArr_remove(&arr, 5);
    int* num = BArr_get(&arr, 5);

    EQ(*num, 6, "Array element 5 not properly removed");

    PASS;
}

// Macro heavy tests

TEST(array_iterate) {
    BArr arr = MAKE_ARR(int);

    for (int i = 0; i < 10; i++) {
        BArr_push(&arr, &i);
    }

    FOR_EACH_ARR(i, int, elem, arr) {
        EQ(i, *elem, "Array contents are not being properly iterated over");
    }

    PASS;
}

TEST(array_push2) {
    BArr arr = MAKE_ARR(char*);

    ARR_PUSH(arr, (char*)"Hello");
    ARR_PUSH(arr, (char*)"World");

    STR_EQ(*(char**)BArr_get(&arr, 0), "Hello", "Item not properly stored");
    STR_EQ(*(char**)BArr_get(&arr, 1), "World", "Item not properly stored");

    PASS;
}

