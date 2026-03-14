#include "defines.h"
#include <stdint.h>

typedef struct {
    uint8_t  data[SHRED_SIZE];
    uint32_t index;
    uint8_t  type;
} Shred;

typedef struct {
    Shred data_shred[DATA_SHRED];
    Shred coding_shred[CODING_SHRED];
} ShredSet;
