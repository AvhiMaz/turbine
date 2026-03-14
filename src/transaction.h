#include <stdint.h>

typedef struct {
    uint8_t  raw_bytes[1024];
    uint32_t size;
} Transaction;
