#include "chacha.h"
#include "utils/rdtsc.h"

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

static const uint8_t ROUNDS[3] = {8, 12, 20};

int main(void) {
  static const uint64_t N = 5000;
  static const uint64_t LENGTH = 32768 * 2;
  uint64_t acc = 0;
  uint8_t* buf = valloc(LENGTH * 2);
  uint8_t* out = buf + LENGTH;
  uint8_t* in = buf;
  uint8_t* key = buf;
  uint8_t* nonce = buf;
  for (size_t j = 0; j < 3; j++) {
    uint8_t rounds = ROUNDS[j];
    for (size_t i = 0; i < N; i++) {
      uint64_t start = rdtsc();
      chacha_xor(out, in, LENGTH, nonce, key, rounds);
      uint64_t total = rdtsc() - start;
      uint64_t rdtsc_overhead = 0;  // rdtsc();
      // rdtsc_overhead = rdtsc() - rdtsc_overhead;
      acc += total - rdtsc_overhead;
      uint8_t* temp = in;
      in = out;
      out = temp;
    }
    printf("%u acc/%llu=%f\n",
           rounds,
           N,
           ((double)acc) / ((double)N) / ((double)LENGTH));
  }
}
