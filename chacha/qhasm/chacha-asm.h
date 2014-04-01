#ifndef CHACHA_ASM_H
#define CHACHA_ASM_H
#include <stdint.h>

int chacha_keystream_bytes(uint32_t* x, uint8_t* out, size_t bytes);
int chacha_xor_bytes(uint32_t* x, uint8_t* m, uint8_t* out, size_t bytes);

#endif

