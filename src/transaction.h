#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "defines.h"
#include <stdint.h>

typedef struct {
    uint8_t  raw_bytes[DATA_SHRED * SHRED_SIZE];
    uint32_t size;
} Transaction;

#endif
