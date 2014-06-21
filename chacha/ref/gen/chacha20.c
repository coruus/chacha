#include "chacha.h"
#include "ref/ops.h"
#include "ref/constants.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "./chacha20.gen.c"

/* Generate a keystream from a ChaCha state and XOR it with the
 * output.
 *
 * Note that chacha_state is modified to reflect the number of
 * blocks copied; this function can thus be used to partially
 * process a stream and then resume.
 */
int chacha_state_chacha20(uint32_t chacha_state[16],
                          uint8_t* const restrict out,
                          const uint8_t* const restrict in,
                          register const size_t inlen) {
  // assert(out != in && "Aliasing assumption violated.");

  if (inlen == 0) {
    return -1;
  } else if (inlen >= chacha_maxlen) {
    return -1;
  }

  register size_t remaining = inlen;
  register size_t outpos = 0;
  register size_t inpos = 0;

  uint32_t x[16] = {0};
  do {
    // Copy the state back into the working copy.
    memcpy(x, chacha_state, 16 * 4);
    _do_chacha20(x, chacha_state);
    register size_t oplen;
    // Set oplen, while avoiding overflow.
    if ((inlen - inpos) < 64) {
      oplen = inlen - inpos;
    } else {
      oplen = 64;
    }
    xor(out + outpos, in + inpos, (uint8_t*)x, oplen);

    // Increment the 64-bit counter.
    chacha_state[12] += 1;
    if (chacha_state[12] == 0) {
      // The second block is incremented only after 2^32 bytes.
      chacha_state[13] += 1;
    }

    // No unsigned underflow occurs:
    //@ assert (remaining - oplen) >= 0;
    remaining -= oplen;
    inpos += oplen;
    outpos += oplen;
  } while (remaining != 0);

  return 0;
}
