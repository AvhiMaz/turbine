#include "shred.h"
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
    for (int i = 0; i < DATA_SHRED; i++) {
        // need to impl XOR here to get recoverable coding shred
        // memcpy(set->coding_shred[i].data, a ^ b, 128);
        set->coding_shred[i].index = i;
        set->coding_shred[i].type = SHRED_CODING;
    }
}
