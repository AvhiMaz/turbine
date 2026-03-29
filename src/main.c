#include "../thirdparty/b64/b64.h"
#include "../thirdparty/cjson/cJSON.h"
#include "../wrapper/rs_wrapper.h"
#include "shred.h"
#include "thread_pool.h"
#include "transaction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    char *buf = malloc(size + 1);
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return buf;
}

int main() {
    char *json_str = read_file("data/transactions.json");
    if (!json_str) {
        printf("failed to open data/transactions.json\n");
        return 1;
    }

    cJSON *json = cJSON_Parse(json_str);
    free(json_str);
    if (!json) {
        printf("failed to parse JSON\n");
        return 1;
    }

    int        total = cJSON_GetArraySize(json);
    int        recovered = 0;
    int        failed = 0;

    ThreadPool tp;
    tp_init(&tp);
    tp_start(&tp);

    for (int t = 0; t < total; t++) {
        cJSON      *item = cJSON_GetArrayItem(json, t);
        const char *b64 = item->valuestring;

        size_t      decoded_len = 0;
        uint8_t    *raw = b64_decode_ex(b64, strlen(b64), &decoded_len);
        if (!raw)
            continue;

        Transaction tx;
        memset(tx.raw_bytes, 0, sizeof(tx.raw_bytes));
        size_t copy_len = decoded_len < sizeof(tx.raw_bytes)
                              ? decoded_len
                              : sizeof(tx.raw_bytes);
        memcpy(tx.raw_bytes, raw, copy_len);
        tx.size = copy_len;
        free(raw);

        ShredSet set;
        shred(&tx, &set);
        generate_coding_shred(&set);

        for (int i = 0; i < DATA_SHRED; i++)
            tp_submit(&tp, &set.data_shred[i]);
        for (int i = 0; i < CODING_SHRED; i++)
            tp_submit(&tp, &set.coding_shred[i]);

        tp_wait(&tp);

        uint8_t saved[SHRED_SIZE];
        memcpy(saved, set.data_shred[2].data, SHRED_SIZE);
        memset(set.data_shred[2].data, 0, SHRED_SIZE);

        void *data_shreds[DATA_SHRED];
        void *coding_shreds[CODING_SHRED];
        for (int i = 0; i < DATA_SHRED; i++) {
            data_shreds[i] = set.data_shred[i].data;
            coding_shreds[i] = set.coding_shred[i].data;
        }

        int lost_indices[] = {2};
        int r = rs_decode(DATA_SHRED, SHRED_SIZE, data_shreds, coding_shreds,
                          lost_indices, 1);
        memcpy(set.data_shred[2].data, data_shreds[2], SHRED_SIZE);

        if (r == 0 && memcmp(set.data_shred[2].data, saved, SHRED_SIZE) == 0)
            recovered++;
        else
            failed++;
    }

    tp_shutdown(&tp);
    cJSON_Delete(json);

    printf("total:     %d\n", total);
    printf("recovered: %d\n", recovered);
    printf("failed:    %d\n", failed);

    return 0;
}
