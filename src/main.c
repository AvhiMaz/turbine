#include "../wrapper/rs_wrapper.h"
#include "shred.h"
#include "transaction.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main() {
    Transaction tx;
    memset(tx.raw_bytes, 0xAB, sizeof(tx.raw_bytes));
    tx.size = sizeof(tx.raw_bytes);

    ShredSet set;
    shred(&tx, &set);

    generate_coding_shred(&set);

    uint8_t saved[SHRED_SIZE];
    memcpy(saved, set.data_shred[2].data, SHRED_SIZE);
    memset(set.data_shred[2].data, 0, SHRED_SIZE);

    void *data_shreds[DATA_SHRED];
    void *coding_shreds[CODING_SHRED];

    int   lost_indices[] = {2};
    int   lost_count = 1;

    for (int i = 0; i < DATA_SHRED; i++) {
        data_shreds[i] = set.data_shred[i].data;
        coding_shreds[i] = set.coding_shred[i].data;
    }

    int r = rs_decode(DATA_SHRED, SHRED_SIZE, data_shreds, coding_shreds,
                      lost_indices, lost_count);

    memcpy(set.data_shred[2].data, data_shreds[2], SHRED_SIZE);

    assert(r == 0);

    if (memcmp(set.data_shred[2].data, saved, SHRED_SIZE) == 0)
        printf("recovery OK\n");
    else
        printf("recovery FAILED\n");

    for (int i = 0; i < DATA_SHRED; i++) {
        int ok = validate_shred(&set.data_shred[i]);
        printf("data_shred[%d] %s\n", i, ok ? "OK" : "CORRUPT");
    }
    for (int i = 0; i < CODING_SHRED; i++) {
        int ok = validate_shred(&set.coding_shred[i]);
        printf("coding_shred[%d] %s\n", i, ok ? "OK" : "CORRUPT");
    }

    return 0;
}
