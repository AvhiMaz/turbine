#include "leopard.h"
#include <cstdlib>
#include <cstring>

int rs_encode(int count, int bytes, void **data_shreds, void **coding_shreds) {
    if (leo_init() != 0) {
        return -1;
    }

    unsigned work_count = leo_encode_work_count(count, count);
    void   **work = (void **)malloc(work_count * sizeof(void *));

    for (unsigned i = 0; i < work_count; i++) {
        work[i] = malloc(bytes);
    };

    LeopardResult r =
        leo_encode(bytes, count, count, work_count, data_shreds, work);

    if (r == Leopard_Success) {
        for (int i = 0; i < count; i++) {
            memcpy(coding_shreds[i], work[i], bytes);
        }
    }

    for (unsigned i = 0; i < work_count; i++) {
        free(work[i]);
    }
    free(work);

    return r == Leopard_Success ? 0 : -1;
}
