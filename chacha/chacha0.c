#include "chacha.h"
#include "utils/print-state-impl.h"
#include <stdint.h>

int main(void) {
  uint32_t x[16] = {0};
  uint32_t s[16] = {1};
  uint32_t i[16] = {0};
  uint32_t o[16] = {0};
  chacha_state_chacha8(s, o, i, 4 * 16);
  print_state(o);
}
