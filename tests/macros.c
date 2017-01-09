
#include "test-helper.h"

#include <stdio.h>

#include "bass/base.h"
#include "bass/arr.h"
#include "bass/types.h"

TEST(macro_let) {
    AUTO x = 42;

    EQ(x, 42, "x was not properly set");
    PASS;
}

TEST(macro_lambda) {
    AUTO f = LAMBDA(i32, (i32 x) {
        return x * x;
    });

    EQ(f(5), 25, "f was not defined correctly");
    PASS;
}
