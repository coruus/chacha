#include "chacha.h"
#include "utils.h"

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

int chacha_xor(uint8_t* out,
               const uint8_t* in,
               register size_t inlen,
               const uint8_t* restrict nonce,
               const uint8_t* restrict key,
               register const size_t rounds) {
  switch (rounds) {
  case 8:
    return chacha8_xor(out, in, inlen, nonce, key, rounds);
  case 12:
    return chacha12_xor(out, in, inlen, nonce, key, rounds);
  case 20:
    return chacha20_xor(out, in, inlen, nonce, key, rounds);
  default:
    assert(0 && "Only 8, 12, or 20 rounds are supported by this implementation.");
    return -1;
  }
  __unreachable();
}
