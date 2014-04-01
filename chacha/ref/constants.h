#ifndef CHACHA_REF_CONSTANTS_H
#define CHACHA_REF_CONSTANTS_H
#include <stdint.h>
#include <stdlib.h>
static const uint32_t chacha256bit_constants[4] = {1634760805, 857760878,
                                                   2036477234, 1797285236};
static const size_t chacha_maxlen = SIZE_MAX >> 1L;
#endif
