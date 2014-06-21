#include "vec/parallel.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  uint8_t out[16 * 16] = {0};
  uint8_t key[32] = {0};
  uint8_t nonce[32] = {0};
  uint32_t pos[2] = {0, 0};
  chacha4block(out, NULL, key, nonce, pos);
  printf("%x %x %x %x\n", out[0], out[1], out[2], out[3]);
}
