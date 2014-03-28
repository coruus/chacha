#include "chacha.h"
#include "tests/kats/chacha-kat-strombergson.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline int bytediff(const uint8_t* const restrict in1,
                           const uint8_t* const restrict in2,
                           const size_t inlen) {
  static const size_t distance = 8;

  size_t i = 0;
  for (; i < inlen; i++) {
    if (in1[i] != in2[i]) {
      goto printdiff;
    }
  }
  return 0;
  register size_t mini, maxi;

printdiff:
  if (i >= distance) {
    mini = i - distance;
  } else {
    mini = 0;
  }
  if ((i + distance) < inlen) {
    maxi = i + distance;
  } else {
    maxi = inlen - 1;
  }
  for (register size_t j = mini; j < maxi; j++) {
    printf("%zu: 0x%02x 0x%02x\n", j, in1[j], in2[j]);
  }
  return -1;
}

static inline void bswap(uint32_t* io, size_t size) {
  for (size_t i = 0; i < size; i++) {
    io[i] = __builtin_bswap32(io[i]);
  }
}

int check_case(const uint8_t key[32],
               const uint8_t nonce[8],
               const uint8_t expected0[64],
               const uint8_t expected1[64],
               register const size_t rounds,
               const char* const restrict name) {
  printf("TC %s: ", name);
  const uint8_t zeros[128] = {0};
  uint8_t buf[128] = {0};
  uint8_t expected[128];
  memcpy(expected, expected0, 64);
  memcpy(expected + 64, expected1, 64);

  // Check that copy did occur.
  if (memcmp(buf, expected, 128) == 0) {
    printf("COPY FAILED\n");
  }
  chacha_xor(buf, zeros, 128, nonce, key, rounds);
  int ret = memcmp(buf, expected, 128);
  if (ret != 0) {
    printf("KS GEN FAILED\n");
    bytediff(buf, expected, 128);
    return ret;
  } else {
    printf("KS GEN PASS ");
  }
  chacha_xor(buf, expected, 128, nonce, key, rounds);
  ret = memcmp(buf, zeros, 128);
  if (ret != 0) {
    printf("KS RGEN FAILED\n");
    bytediff(buf, expected, 128);
    return ret;
  } else {
    printf("KS RGEN PASS\n");
  }
  return 0;
}

int main(void) {
  register int ret = 0;
#include "strombergson-cases.gen.c"
  if (ret) {
    return -1;
  } else {
    return 0;
  }
}
