
#include "test-helper.h"

#include "bass/map.h"

TEST(map_init) {
    BMap map = MAKE_MAP(int, 1024);

    BMap_clean(&map);
    PASS;
}

TEST(map_put) {
    BMap map = MAKE_MAP(int, 1024);

    BMap_put(&map, "X", (int[]) { 0 });

    BMap_clean(&map);
    PASS;
}

TEST(map_get) {
    BMap map = MAKE_MAP(int, 1024);

    BMap_put(&map, "X", (int[]) { 42 });
    int* x = BMap_get(&map, "X");

    NOT_NULL(x, "X was not set");
    EQ(*x, 42, "X was not retrieved properly");

    BMap_clean(&map);
    PASS;
}

TEST(map_iterate) {
    BMap map = MAKE_MAP(int, 64);

    BMap_put(&map, "x", (int[]) { 1 });
    BMap_put(&map, "data", (int[]) { 10 });
    BMap_put(&map, "LONG ASS STRING WITH SPACES", (int[]) { 128 });
    BMap_put(&map, "xXx_los_cummies_xXx", (int[]) { 42 });

    int count = 0;
    BMapIterCode callback(char* key, void* value) {
        count++;
        return Continue;
    }

    BMap_iter(&map, callback);
    EQ(count, 4, "Map iteration is brokered");

    PASS;
}
