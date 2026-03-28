#include "../wrapper/rs_wrapper.h"
#include "shred.h"
#include "thread_pool.h"
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

    ThreadPool tp;
    tp_init(&tp);
    tp_start(&tp);

    for (int i = 0; i < DATA_SHRED; i++)
        tp_submit(&tp, &set.data_shred[i]);
    for (int i = 0; i < CODING_SHRED; i++)
        tp_submit(&tp, &set.coding_shred[i]);

    tp_wait(&tp);
    tp_shutdown(&tp);

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

    return 0;
}
