#include "chacha.h"
#include "tests/check-kat.h"
#include "tests/kats/chacha-kat-strombergson.h"

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
               const char* const restrict name) {
  printf("TC %s: ", name);

  uint8_t expected[128];
  memcpy(expected, expected0, 64);
  memcpy(expected + 64, expected1, 64);

  return check_kat(key, nonce, expected, 128, rounds, name);
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
