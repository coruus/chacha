#include "chacha.h"
#include "ref/xor.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define rol(v, c) ((v << c) | (v >> (32 - c)))

static const uint32_t chacha256bit_constants[4] = { 1634760805, 857760878, 2036477234, 1797285236 };

int chacha12_xor(uint8_t* const restrict out,
               const uint8_t* const restrict in,
               register const size_t inlen,
               const uint8_t* const restrict nonce,
               const uint8_t* const restrict key) {
  assert(out != in && "Pointer aliasing!!!");
  register size_t outpos = 0;
  register size_t inpos = 0;
  uint32_t input[16] = { 0 };
  memcpy(input, chacha256bit_constants, 4 * 4);
  memcpy(input + 4, key, 32);
  memcpy(input + 14, nonce, 8);
  input[12] = 0;
  input[13] = 0;
  uint32_t x[16] = { 0 };
  /*for (size_t i = 0; i < 16; i++) {
    printf("input[%u]=%x\n", i, input[i]);
  }*/
  register size_t remaining = inlen;
  do {
    memcpy(x, input, 16 * 4);
     #include "chacha12.gen.c"
    //memcpy(out, x, 16 * sizeof(uint32_t));
    register size_t oplen = ((inpos + 64) >= inlen) ? (inlen - inpos) : 64;
    xor(out + outpos, in + inpos, (uint8_t*)x, oplen);
    input[12] += 1;
    if (input[12] == 0) {
      input[13] += 1;
    }
    remaining -= oplen;
    inpos += oplen, outpos += oplen;
  } while (remaining != 0);

  return 0;
}
