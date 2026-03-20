#include "../thirdparty/leopard.h"
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

int rs_decode(int count, int bytes, void **data_shreds, void **coding_shreds,
              int *lost_indices, int lost_count) {

    if (leo_init() != 0) {
        return -1;
    }

    for (int i = 0; i < lost_count; i++) {
        int idx = lost_indices[i];
        if (idx < count)
            data_shreds[idx] = NULL;
        else
            coding_shreds[idx - count] = NULL;
    }

    unsigned work_count = leo_decode_work_count(count, count);
    void   **work = (void **)malloc(work_count * sizeof(void *));

    for (unsigned i = 0; i < work_count; i++) {
        work[i] = malloc(bytes);
    };

    LeopardResult r = leo_decode(bytes, count, count, work_count, data_shreds,
                                 coding_shreds, work);

    if (r == Leopard_Success) {
        for (int i = 0; i < lost_count; i++) {
            int idx = lost_indices[i];
            if (idx < count) {
                memcpy(data_shreds[idx], work[idx], bytes);
            }
        }
    }

    for (unsigned i = 0; i < work_count; i++) {
        free(work[i]);
    }
    free(work);

    return r == Leopard_Success ? 0 : -1;
}
