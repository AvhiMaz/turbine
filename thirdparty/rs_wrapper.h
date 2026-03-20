#ifndef RS_WRAPPER_H
#define RS_WRAPPER_H

#ifdef __cplusplus

extern "C" {
#endif

int rs_encode(int count, int bytes, void **data_shreds, void **coding_shreds);

#ifdef __cplusplus
}
#endif

#endif
