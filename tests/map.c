#include "test-helper.h"

#include "bass/base.h"
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
    BMap_put(&map, "LONG", (int[]) { 128 });
    BMap_put(&map, "fuck", (int[]) { 128 });

    int count = 0;
    BMap_iter(&map, LAMBDA(BMapIterCode, (char* key, void* value) {
        count++;
        return Continue;
    }));

    EQ(count, 4, "Map iteration is brokered");

    BMap_clean(&map);
    PASS;
}

TEST(map_struct) {
    typedef struct {
        int x, y, z;
        float real;
        char* text;
    } Data;

    BMap map = MAKE_MAP(Data, 64);

    BMap_put(&map, "data", (Data[]) {{
        .x = 10,
        .y = 100,
        .z = 42,

        .real = 0.5,
        .text = "TEXT!",
    }});

    Data* data = BMap_get(&map, "data");

    NOT_NULL(data, "Unable to retrieve data");

    EQ(data->x, 10, "Data not properly stored");
    EQ(data->y, 100, "Data not properly stored");
    EQ(data->z, 42, "Data not properly stored");

    EQ(data->real, 0.5, "Data not properly stored");
    STR_EQ(data->text, "TEXT!", "Data not properly stored");

    BMap_clean(&map);
    PASS;
}

TEST(map_collision) {
    AUTODEL(BMap) map = MAKE_MAP(int, 32);

    char* key1 = "altarages";
    char* key2 = "zinkes";

    u64 hash1 = hash_fnv1a(key1) & (map.total_pairs - 1);
    u64 hash2 = hash_fnv1a(key2) & (map.total_pairs - 1);
    EQ(hash1, hash2, "Hashes are not equal, cannot test collisions");

    BMap_put(&map, key1, (int[]) { 1 });
    BMap_put(&map, key2, (int[]) { 2 });

    int* val1 = BMap_get(&map, key1);
    int* val2 = BMap_get(&map, key2);

    NOT_NULL(val1, "Unable to retrieve value");
    NOT_NULL(val2, "Unable to retrieve value");

    EQ(1, *val1, "Improperly retrieved value");
    EQ(2, *val2, "Improperly retrieved value");

    PASS;
}
