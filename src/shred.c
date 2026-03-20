#include "shred.h"
#include "../wrapper/rs_wrapper.h"
#include "defines.h"
#include "transaction.h"
#include <string.h>

void shred(Transaction *tx, ShredSet *set) {

    for (int i = 0; i < DATA_SHRED; i++) {
        memcpy(set->data_shred[i].data, tx->raw_bytes + (i * SHRED_SIZE),
               SHRED_SIZE);
        set->data_shred[i].index = i;
        set->data_shred[i].type = SHRED_DATA;
    }
}

void generate_coding_shred(ShredSet *set) {

    void *data_shreds[DATA_SHRED];
    void *coding_shreds[CODING_SHRED];

    for (int i = 0; i < DATA_SHRED; i++) {
        data_shreds[i] = set->data_shred[i].data;
        coding_shreds[i] = set->coding_shred[i].data;
    }

    rs_encode(DATA_SHRED, SHRED_SIZE, data_shreds, coding_shreds);
}
