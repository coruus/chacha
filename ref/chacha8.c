#include "chacha.h"
#include "ref/ops.h"
#include "ref/constants.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "chacha8.gen.c"

/*@ requires \valid(out + (0..inlen-1)) && \valid(in + (0..inlen-1))
 *           && \valid(nonce + (0..8)) && \valid(key + (0..32));
 *  behavior failure:
 *    assumes inlen == 0 || inlen >= chacha_maxlen;
 *    ensures \return == -1;
 *    assigns \nothing;
 *  behavior success:
 *    assumes inlen != 0 && inlen < chacha_maxlen;
 *    ensures \return == 0;
 *    assigns out[0..inlen-1];
 *  complete behaviors failure, success;
 *  disjoint behaviors fails, success;
 */
int chacha8_xor(uint8_t* const restrict out,
                const uint8_t* const restrict in,
                register const size_t inlen,
                const uint8_t* const restrict nonce,
                const uint8_t* const restrict key) {
  assert(out != in);

  if (inlen == 0) {
    return -1;
  } else if (inlen >= chacha_maxlen) {
    return -1;
  }

  register size_t remaining = inlen;
  register size_t outpos = 0;
  register size_t inpos = 0;

  // Initialize the state.
  uint32_t input[16] = {0};
  memcpy(input, chacha256bit_constants, 16);
  memcpy(input + 4, key, 32);
  memcpy(input + 14, nonce, 8);
  input[12] = 0;
  input[13] = 0;

  uint32_t x[16] = {0};
  do {
    //@ loop variant remaining;
    //@ assert (inpos < inlen) && (outpos < outlen);

    // Copy the initial state back into the working copy.
    memcpy(x, input, 16 * 4);
    _do_chacha8(x, input);
    register size_t oplen;
    // Set oplen, while avoiding overflow.
    if ((inlen - inpos) < 64) {
      //@ assert (inlen - inpos) > 0;
      oplen = inlen - inpos;
    } else {
      //@ assert (inlen - inpos) >= 64;
      oplen = 64;
    }
    // The xor uses only valid addresses.
    /*@ assert \valid(out+outpos..out+outpos+oplen-1)
     *      && \valid(in+inpos..in+inpos+oplen-1);
     */
    xor(out + outpos, in + inpos, (uint8_t*)x, oplen);

    // Increment the 64-bit counter.
    input[12] += 1;
    if (input[12] == 0) {
      // The second block is incremented only after 2^32 bytes.
      //@ assert inpos >= (1 << 32);
      input[13] += 1;
    }

    // No unsigned underflow occurs:
    //@ assert (remaining - oplen) >= 0;
    remaining -= oplen;
    inpos += oplen;
    outpos += oplen;
  } while (remaining != 0);

  // We have performed the full operation.
  //@ assert (inpos == inlen) && (outpos == inlen);

  return 0;
}
