#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "utils/print-state-impl.h"

// static const uint8_t r[4] = {16, 12, 8, 7};
static const uint8_t r[4] = {16, 8, 12, 7};
/*
int chacha(uint32_t in[16], uint8_t rounds) {
  uint32_t x[16] = { 1 };
  for (int dr = 0; dr < rounds; dr += 2) {
    for (int joff = 4; joff <= 5; joff++) {
      for (int i = 0; i < 4; i += 1) {
        for (int j = 0; j < 16; j += joff) {
          // printf("%2u %2u %2u   %2u\n", joff, i, j, (i + j)&15);
          for (int k = 0, ri = 0; k < 16; k += 8, ri = (ri + 1) & 3) {
            int ix = (i + j + k);
            int a = (ix + 0) & 15, b = (ix + 4) & 15, d = (ix + 12) & 15;
            printf("%2u %2u %2u %2u %2u\n", joff, a, b, d, r[ri]);
            x[a] += x[b];
            x[d] ^= x[a];
            x[d] = (x[d] << r[ri]) | (x[d] >> r[ri]);
          }
        }
      }
    }
  }
  for (int i = 0; i < 16; i++) {
    in[i] += x[i];
  }
  return 0;
}
*/
/*
int chacha(uint32_t in[16], uint8_t rounds) {
  int yoff = 4;
  uint32_t x[16] = { 1 };
  for (int round = 0; round < rounds; round += 2) {
  for (int yoff = 4; yoff < 6; yoff++) {
    for (int ix = 0; ix < 4; ix++) {
      for (int iz = 0; iz < 2; iz++){
      for (int iy = 0, iyoff=0; iy < 4; iy+=2, iyoff+=yoff) {
        int ia = ix + iyoff;
        int ib = ia + yoff;
        int id = (ib + yoff + yoff) & 15;
        int ri = r[(iz + iy) & 3];
        x[ia] += x[ib];
        x[id] ^= x[ia];
        x[id] = (x[id] << ri) | x[id] >> (32 - ri);
        printf("   %2u,%2u,%2u<<<%2u", ia, ib, id, ri);
      }
    }
      printf("\n");
    }
  }
  }
  for (int i = 0; i < 16; i++) {
    in[i] += x[i];
  }
  return 0;
}
*/

#define S(a, b, d, r) \
  a += b;             \
  d ^= a;             \
  d = ((d << r) | (d >> (32 - r)));

#define QR(a, b, c, d) \
  S(a, b, d, 16)       \
  S(c, d, b, 12)       \
  S(a, b, d, 8)        \
  S(c, d, b, 7)

#define QRX(ia, ib, ic, id) QR(X(ia), X(ib), X(ic), X(id))

#define X(i) x[(i)&15]

int chacha(uint32_t in[16], uint8_t rounds) {
  uint32_t x[16] = {1};
  for (int round = 0; round < rounds; round++) {
    for (int ystep = 4; ystep < 6; ystep++) {
      for (int ix = 0; ix < 4; ix++) {
        QRX(ix + ystep, ix + ystep * 2, ix + ystep * 3, ix + ystep * 4);
      }
    }
  }
  for (int i = 0; i < 16; i++) {
    in[i] += x[i];
  }
  return 0;
}

int main(void) {
  uint32_t x[16] = {0};
  chacha(x, 8);
  print_state(x);
}
