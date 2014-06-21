#include <stdlib.h>
#include <stdint.h>

static const uint8_t rhos[4] = {16, 12, 8, 7};

static inline uint32_t rol(register const uint32_t n,
                           register const uint8_t shift) {
  return (n << shift) | (n >> (32 - shift));
}

for (uint8_t i = 0; i < 4; i++) {
  uint8_t off = (i * (2 * xoff)) % 16;

  0, 4, 12 8, 12, 4 0, 4, 12 8, 12, 4 a += b;
  d ^= a;
  d = rol(d, 16);
