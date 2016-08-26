
#pragma once

typedef struct {
    u8* buf;

    i64 len;
    i64 capacity;
} BString;

void BStringInit(BString*);
bool BStringClean(BString*);


