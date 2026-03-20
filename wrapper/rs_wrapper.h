#ifndef RS_WRAPPER_H
#define RS_WRAPPER_H

#ifdef __cplusplus

extern "C" {
#endif

int rs_encode(int count, int bytes, void **data_shreds, void **coding_shreds);
int rs_decode(int count, int bytes, void **data_shreds, void **coding_shreds,
              int *lost_indices, int lost_count);

#ifdef __cplusplus
}
#endif

#endif
