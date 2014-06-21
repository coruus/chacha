#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const uint8_t sigma[16] = "expand 32-byte k";
static const uint8_t tau[16] = "expand 16-byte k";

inline int chacha_state_reset(uint32_t chacha_state[16],
                              register const uint8_t* const restrict nonce,
                              register const size_t stream_position) {
  uint64_t position = stream_position;
  memcpy(chacha_state + 14, position, 8);
  memcpy(chacha_state + 14, nonce, 8);
  return 0;
}

int chacha_state_init(uint32_t chacha_state[16],
                      register const uint8_t* const restrict key,
                      register const uint8_t* const restrict nonce,
                      register const size_t stream_position,
                      register const size_t keybitlen) {
  switch (keybitlen) {
    case 256:
      goto setup256;
    case 128:
      goto setup128;
    default:
      return -1;
  }

setup256:
  memcpy(chacha_state, sigma, 16);
  memcpy(chacha_state + 4, key, 32);
  goto setposition;

setup128:
  memcpy(chacha_state, tau, 16);
  memcpy(chacha_state + 4, key, 16);
  memcpy(chacha_state + 8, key, 16);

setposition:
  chacha_state_reset(chacha_state, nonce, stream_position);

  return 0;
}
