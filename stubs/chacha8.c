#include "chacha.h"
#include "utils/clean.h"

#include <stdint.h>

int chacha8_xor(register uint8_t* out,
                register const uint8_t* in,
                register size_t oplen,
                const uint8_t* restrict nonce,
                const uint8_t* restrict key) {
  // Allocate page-aligned memory for the state.
  uint32_t* chacha_state = valloc(16 * 4);
  if (chacha_state == NULL) {
    return -1;
  }

  int err = 0;
  err = chacha_state_init(chacha_state, key, nonce, 0, 256);
  if (err) {
    goto cleanup;
  }
  err = chacha_state_chacha8(chacha_state, out, in, oplen);
  if (err) {
    goto cleanup;
  }
  return err;

cleanup:
  clearstate(chacha_state);
  free(chacha_state);
  chacha_state = NULL;
  return err;
}
