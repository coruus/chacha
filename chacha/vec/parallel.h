#ifndef VEC_PARALLEL_H
#define VEC_PARALLEL_H
extern void chacha4block(void* out,
                         void* in,
                         void* key,
                         void* nonce,
                         void* position);
#endif  // VEC_PARALLEL_H
