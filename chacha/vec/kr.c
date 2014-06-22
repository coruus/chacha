//#include <stdint.h>
//#include <stdlib.h>
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef uint32_t size_t;
typedef unsigned long long uint64_t;

#ifndef __clang__
#define vecrol1(v) __builtin_shuffle(v, (v4ui){1, 2, 3, 0})
#define vecrol2(v) __builtin_shuffle(v, (v4ui){2, 3, 0, 1})
#define vecrol3(v) __builtin_shuffle(v, (v4ui){3, 0, 1, 2})
#else  // defined(__clang__)
#define vecrol1(v) __builtin_shufflevector(v, v, 1, 2, 3, 0)
#define vecrol2(v) __builtin_shufflevector(v, v, 2, 3, 0, 1)
#define vecrol3(v) __builtin_shufflevector(v, v, 3, 0, 1, 2)
#endif

#define rol(s, val) val = ((val << (v4ui){s, s, s, s}) | (val >> (v4ui){32-s,32-s,32-s,32-s}))
#define rol16(x) rol(16, x)
#define rol12(x) rol(12, x)
#define rol8(x) rol(8, x)
#define rol7(x) rol(7, x)


/* If ALLOW_UNDEFINED_BEHAVIOR=1 and CRYPTO_chacha_20 is passed a pointer which
 * isn't 16-byte aligned for out, in, nonce, or key, the consequences may be dire.
 * (In particular, on x86_64: the assembler should be verified to check alignment.
 * If it doesn't, insidiously rare segfaults will occur with alignment-check on.)
 *
 * The option is only provided because some versions of Clang do not inline
 * memcpy to an alignment check on this code.
 */
#define ALLOW_UNDEFINED_BEHAVIOR 1

#if !defined(ALLOW_UNDEFINED_BEHAVIOR) || (ALLOW_UNDEFINED_BEHAVIOR == 0)
#include <string.h>
#endif

typedef uint32_t v4ui __attribute__((__vector_size__((4*4))));

// Definitions of ChaCha

#define quarterround(a, b, c, d) \
  a += b; d ^= a; rol16(d);      \
  c += d; b ^= c; rol12(b);      \
  a += b; d ^= a; rol8(d);       \
  c += d; b ^= c; rol7(b);       \

#define doubleround_vec(a, b, c, d)               \
  quarterround(a, b, c, d)                        \
  b = vecrol1(b); c = vecrol2(c); d = vecrol3(d); \
  quarterround(a, b, c, d)                        \
  b = vecrol3(b); c = vecrol2(c); d = vecrol1(d);

#define chacha_core(x) doubleround_vec(x[0], x[1], x[2], x[3])

#define repeat10(X) do { \
  X X X X X              \
  X X X X X            } while (0)

#define chachablock(x)                                  \
    x[0] = state[0]; x[1] = state[1];                   \
    x[2] = state[2]; x[3] = state[3];                   \
    repeat10(doubleround_vec(x[0], x[1], x[2], x[3]));  \
    x[0] += state[0]; x[1] += state[1];                 \
    x[2] += state[2]; x[3] += state[3];

#define load_counter()                         \
  do {                                         \
  state[3] = (v4ui){(uint32_t)(&counter)[0],   \
                    (uint32_t)(&counter)[1],   \
                    anonce[0], anonce[1]};     \
  while (0)

#define _xorblock_inaligned(out, x)               \
    do {                                          \
    out[0] = ((v4ui*)in)[0] ^ (x[0] + state[0]);  \
    out[1] = ((v4ui*)in)[1] ^ (x[1] + state[1]);  \
    out[2] = ((v4ui*)in)[2] ^ (x[2] + state[2]);  \
    out[3] = ((v4ui*)in)[3] ^ (x[3] + state[3]);  \
    while (0)

#define _xorblock_inrealign(out, x)          \
    do {                                     \
    v4ui t[4];                               \
    memcpy(t, in, max(inlen, blocklen));     \
    t[0] ^= x[0]; t[1] ^= x[1];              \
    t[2] ^= x[2]; t[3] ^= x[3];              \
    memcpy(out, t, max(inlen, blocklen));    \
    } while (0)

#if !defined(ALLOW_UNDEFINED_BEHAVIOR) || (ALLOW_UNDEFINED_BEHAVIOR == 0)
#define _xorblock _xorblock_inrealign
#else
#define _xorblock _xorblock_inaligned
#endif

#define xorblock(x) _xorblock(((v4ui*)out), x)

void CRYPTO_chacha_20(uint8_t *out,
                      const uint8_t *in,
                      size_t inlen,
                      const uint8_t key[32],
                      const uint8_t nonce[8],
                      uint64_t counter) {
  v4ui state[4];
  state[0] = (v4ui){0x61707865, 0x3320646E, 0x79622D32, 0x6B206574};
#if !defined(ALLOW_UNDEFINED_BEHAVIOR) || ALLOW_UNDEFINED_BEHAVIOR==0
  uint32_t *akey = key;
  uint32_t *anonce = nonce;
  memcpy(akey, key, 32);
  memcpy(anonce, nonce, 8);
#else   // defined(ALLOW_UNDEFINED_BEHAVIOR) && ALLOW_UNDEFINED_BEHAVIOR==1
#define akey ((uint32_t*)key)
#define anonce ((uint32_t*)nonce)
#endif
  state[1] = (v4ui){akey[0], akey[1], akey[2], akey[3]};
  state[2] = (v4ui){akey[4], akey[5], akey[6], akey[7]};
  load_counter();

  v4ui x[6][4];

#define blocklen 4 * 4 * 4 * 4
  while (inlen > blocklen) {



#define doblock(i) \
    chachablock(x[i]); xorblock(x[i]); \
    counter += 4; load_counter()
    doblock(0);
    doblock(1);
    doblock(2);
    doblock(3);
    //@assert inlen - blocklen > 0;
    inlen -= blocklen;
  }

  if (inlen) {
#undef doblock
#define doblock(i) \
    chachablock(x[i]); _xorblock(x[i], x[i]); \
    counter += 4; load_counter()
    doblock(0); doblock(1);
    doblock(2); doblock(3);
    for (size_t i = 0; i < inlen; i++) {
      out[i] = in[i] ^ ((uint8_t*)x)[i];
    }
  }
}
