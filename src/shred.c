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
