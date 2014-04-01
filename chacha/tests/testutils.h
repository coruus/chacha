#ifndef TESTUTILS_H
#define TESTUTILS_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define make(TYPE, NAME, LEN)              \
  TYPE* NAME = malloc(LEN * sizeof(TYPE)); \
  if (NAME == NULL) {                      \
    assert(0 && "allocation failure");     \
    abort();                               \
  }                                        \
  memset(NAME, 0, LEN * sizeof(TYPE));

#define del(NAME)     \
  if (NAME != NULL) { \
    free(NAME);       \
    NAME = NULL;      \
  }

#define LOG(message) printf("%s:%u %s\n", __FILE__, __LINE__, message);

static inline bool iszero(const uint8_t* const in, const size_t inlen);
static inline bool iszero(const uint8_t* const in, const size_t inlen) {
  for (size_t i = 0; i < inlen; i++) {
    if (in[i] != 0) {
      return false;
    }
  }
  return true;
}

static inline int bytediff(const uint8_t* const restrict in1,
                           const uint8_t* const restrict in2,
                           const size_t inlen);
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
#endif
