#ifndef REF_OPS_H
#define REF_OPS_H
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
static inline void xor (register uint8_t * const restrict out,
                        register const uint8_t* const restrict in1,
                        register const uint8_t* const restrict in2,
                        register size_t oplen);
static inline void xor (register uint8_t * const restrict out,
                        register const uint8_t* const restrict in1,
                        register const uint8_t* const restrict in2,
                        register size_t oplen) {
                         for (register size_t i = 0; i < oplen; i++) {
                           out[i] = in1[i] ^ in2[i];
                         }
                       }
#define rol(v, c) ((v << c) | (v >> (32 - c)))
#endif
