#ifndef SHRED_H
#define SHRED_H

#include "defines.h"
#include "transaction.h"
#include <stdint.h>

typedef struct {
    uint8_t  data[SHRED_SIZE];
    uint32_t index;
    uint8_t  type;
    uint32_t checksum;
} Shred;

typedef struct {
    Shred data_shred[DATA_SHRED];
    Shred coding_shred[CODING_SHRED];
} ShredSet;

void shred(Transaction *tx, ShredSet *set);
void generate_coding_shred(ShredSet *set);
int  validate_shred(Shred *s);

#endif
