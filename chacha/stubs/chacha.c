#include "chacha.h"
#include "include/utils.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

// sigma = { 0x61707865, 0x3320646e, 0x79622d32, 0x6b206574 }
static const uint8_t sigma[16] = "expand 32-byte k";
static const uint8_t tau[16] = "expand 16-byte k";

/* Re-set a ChaCha position and nonce. If `nonce` is NULL, the current
 * nonce will not be changed.
 */
inline int chacha_state_reset(uint32_t chacha_state[16],
                              register const uint8_t* const restrict nonce,
                              register const size_t stream_position) {
  uint64_t position = stream_position;
  memcpy(chacha_state + 12, &position, 8);
  if (nonce != NULL) {
    memcpy(chacha_state + 14, nonce, 8);
  }
  return 0;
}

int chacha_state_init(uint32_t chacha_state[16],
                      register const uint8_t* const restrict key,
                      register const uint8_t* const restrict nonce,
                      register const size_t stream_position,
                      register const size_t keybitlen) {
  if (nonce == NULL) {
    return -1;
  }
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

/* Multiple-dispatch ChaCha function; intended primarily for benchmarks.
 * Applications should hard-code a specific number of rounds.
 */
int chacha_xor(register uint8_t* out,
               register const uint8_t* in,
               register size_t inlen,
               const uint8_t* restrict nonce,
               const uint8_t* restrict key,
               register const size_t rounds) {

  switch (rounds) {
    case 8:
      return chacha8_xor(out, in, inlen, nonce, key);
    case 12:
      return chacha12_xor(out, in, inlen, nonce, key);
    case 20:
      return chacha20_xor(out, in, inlen, nonce, key);
    default:
      // assert(0 &&
      //       "Only 8, 12, or 20 rounds are supported by this
      //       implementation.");
      return -1;
  }
  __unreachable();
}
