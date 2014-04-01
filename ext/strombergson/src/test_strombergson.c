#include "chacha.h"
#include "tests/kats/chacha-kat-strombergson.h"
#include "tests/testutils.h"
#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int check_case(const uint8_t key[32],
               const uint8_t nonce[8],
               const uint8_t expected0[64],
               const uint8_t expected1[64],
               register const size_t rounds,
               const char* const restrict name);

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
