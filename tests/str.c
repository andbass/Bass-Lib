
#include "test-helper.h"

#include "bass/base.h"
#include "bass/str.h"

TEST(str_basic) {
    autodel(BStr) str = BStr_empty();
    
    STR_EQ(str, "", "Not empty string.  what the butt");
    PASS;
}

TEST(str_from_cstr) {
    autodel(BStr) str = BStr_new("Andrew Bass");

    STR_EQ(str, "Andrew Bass", "String not properly created");
    PASS;
}

TEST(str_format) {
    int pots = 42;
    autodel(BStr) str = BStr_format("I have %d potatos", pots);

    STR_EQ(str, "I have 42 potatos", "Formatted string incorrect");
    PASS;
}

TEST(str_dup) {
    autodel(BStr) original = BStr_new("String is string is string is string");
    autodel(BStr) duped = BStr_new(original);

    STR_EQ(original, duped, "String not duped right");
    PASS;
}

TEST(str_concat) {
    autodel(BStr) str1 = BStr_new("Bobby");
    autodel(BStr) str2 = BStr_new(" Hill");

    autodel(BStr) result = BStr_concat(str1, str2);

    STR_EQ(str1, "Bobby", "Concat param mutated");
    STR_EQ(str2, " Hill", "Concat param mutated");

    STR_EQ(result, "Bobby Hill", "Concat failed");
    PASS;
}

TEST(str_cconcat) {
    autodel(BStr) str = BStr_cconcat("Bobby", " Hill");

    STR_EQ(str, "Bobby Hill", "String concat not worky dorky");
    PASS;
}

TEST(str_eq) {
    autodel(BStr) str1 = BStr_new("this is a long string you boofy");
    autodel(BStr) str2 = BStr_new("this is a long string you boofy");

    EQ(BStr_eq(str1, str2), true, "Strings are not equal");
    PASS;
}

TEST(str_sub) {
    autodel(BStr) str = BStr_new("Andrew the Bass Master");
    autodel(BStr) slice = BStr_sub(str, 11, 15);

    NOT_NULL(slice, "Slice is NULL, we goofed");
    STR_EQ(slice, "Bass", "Slice is not right");

    PASS;
}

TEST(str_sub_end) {
    autodel(BStr) str = BStr_new("xXx the master of it allllll");
    autodel(BStr) slice = BStr_sub_end(str, 21);

    NOT_NULL(slice, "Slice is NULL, nononon");
    STR_EQ(slice, "allllll", "Slice is not right");

    PASS;
}

TEST(str_push) {
    autodel(BStr) str = BStr_new("abcd");

    BStr_push(&str, 'e');
    BStr_push(&str, 'f');
    BStr_push(&str, 'g');
    BStr_push(&str, 'h');

    STR_EQ(str, "abcdefgh", "Can't do ABCs");
    PASS;
}

TEST(str_push_str) {
    autodel(BStr) to = BStr_new("Andrew");
    autodel(BStr) from = BStr_new(" Bass");

    BStr_push_str(&to, from);

    STR_EQ(to, "Andrew Bass", "String push is broken");
    PASS;
}

TEST(str_push_cstr) {
    autodel(BStr) base = BStr_new("xXx");

    for (int i = 0; i < 4; i++) {
        BStr_push_cstr(&base, "xXx");
    }

    STR_EQ(base, "xXxxXxxXxxXxxXx", "C string push is broken");
    PASS;
}

TEST(str_shrink) {
    autodel(BStr) str = BStr_empty();

    for (int i = 0; i < 65; i++) {
        BStr_push(&str, 'a');
    }

    BStr_shrink(&str);
    BStrInternal* str_int = BStr_internal(str);

    EQ(str_int->cap, 66, "String not shrunken");
    PASS;
}

TEST(str_clean_null) {
    autodel(BStr) str = BStr_empty();
    autodel(BStr) slice = BStr_sub(str, 1000, 0);

    PASS; // if we don't segfault, we're fine
}

