#include "chacha.h"
#include "tests/chacha-kat-strombergson.h"
#include "testutils.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define checkinv                                                 \
  err = !iszero(zeros, len);                                     \
  if (err != 0) {                                                \
    LOG("INVARIANT VIOLATED: zeros[i] != 0, /forall i < len\n"); \
    goto fail;                                                   \
  }                                                              \
  err = memcmp(expected, exbuf, len);                            \
  if (err != 0) {                                                \
    LOG("INVARIANT VIOLATED: exbuf[i] != expected[i]\n");        \
    goto fail;                                                   \
  }

int check_kat(const uint8_t key[32],
              const uint8_t nonce[8],
              const uint8_t* const restrict expected,
              register const size_t len,
              register const size_t rounds,
              const char* const restrict name) {
  int err = 0;
  printf("%s: ", name);

  make(uint8_t, zeros, len);
  make(uint8_t, buf, len);
  make(uint8_t, exbuf, len);
  memcpy(exbuf, expected, len);

  checkinv;

  // Check that the keystream matches the expected value.
  chacha_xor(buf, zeros, 128, nonce, key, rounds);
  err = memcmp(buf, expected, 128);
  if (err != 0) {
    printf("FGEN FAIL\n");
    bytediff(buf, expected, 128);
    goto fail;
  } else {
    printf("FGEN PASS ");
  }
  checkinv;

  // Check that xoring the expected value with the keysteam
  // is zero.
  chacha_xor(buf, expected, 128, nonce, key, rounds);
  err = memcmp(buf, zeros, 128);
  if (err != 0) {
    printf("RGEN FAIL\n");
    bytediff(buf, expected, 128);
    goto fail;
  } else {
    printf("RGEN PASS\n");
  }
  checkinv;

  return 0;

fail:
  del(exbuf);
  del(buf);
  del(zeros);
  return -1;
}
