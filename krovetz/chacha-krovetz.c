/* Chacha implementation for 16-byte vectors by Ted Krovetz (ted@krovetz.net).
 * Assumes 32-bit int, 64-bit long long. Public domain. Modified: 2012.07.26.
 * Chacha is an improvement on the stream cipher Salsa, described at
 * http://cr.yp.to/papers.html#chacha
 */
#include "alignment.h"

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Architecture-neutral way to specify 16-byte vector of ints              */
typedef uint32_t vec __attribute__((vector_size(16)));

/* This implementation is designed for Neon, SSE and AltiVec machines. The
 * following specify how to do certain vector operations efficiently on
 * each architecture, using intrinsics.
 * This implementation supports parallel processing of multiple blocks,
 * including potentially using general-purpose registers.
 */
#if __ARM_NEON__
#include "defs-neon.h"
#elif __ALTIVEC__
#error-- AltiVec support not tested; likely to be removed.
#include "defs-altivec.h"
#elif __SSE2__
#include "defs-sse2.h"
#else
#error-- This implementation requires the NEON, AltiVec, or SSE2 instruction set.
#endif

#ifndef REVV_BE
#define REVV_BE(x) (x)
#endif

#ifndef REVW_BE
#define REVW_BE(x) (x)
#endif

#define BPI (VBPI + GPR_TOO) /* Blocks computed per loop iteration   */

#define DQROUND_VECTORS(a, b, c, d) \
  a += b;                           \
  d ^= a;                           \
  d = ROTW16(d);                    \
  c += d;                           \
  b ^= c;                           \
  b = ROTW12(b);                    \
  a += b;                           \
  d ^= a;                           \
  d = ROTW8(d);                     \
  c += d;                           \
  b ^= c;                           \
  b = ROTW7(b);                     \
  b = ROTV1(b);                     \
  c = ROTV2(c);                     \
  d = ROTV3(d);                     \
  a += b;                           \
  d ^= a;                           \
  d = ROTW16(d);                    \
  c += d;                           \
  b ^= c;                           \
  b = ROTW12(b);                    \
  a += b;                           \
  d ^= a;                           \
  d = ROTW8(d);                     \
  c += d;                           \
  b ^= c;                           \
  b = ROTW7(b);                     \
  b = ROTV3(b);                     \
  c = ROTV2(c);                     \
  d = ROTV1(d);

#define QROUND_WORDS(a, b, c, d) \
  a = a + b;                     \
  d ^= a;                        \
  d = d << 16 | d >> 16;         \
  c = c + d;                     \
  b ^= c;                        \
  b = b << 12 | b >> 20;         \
  a = a + b;                     \
  d ^= a;                        \
  d = d << 8 | d >> 24;          \
  c = c + d;                     \
  b ^= c;                        \
  b = b << 7 | b >> 25;

#define WRITE_XOR(in, op, d, v0, v1, v2, v3)               \
  *(vec*)(op + d + 0) = *(vec*)(in + d + 0) ^ REVV_BE(v0); \
  *(vec*)(op + d + 4) = *(vec*)(in + d + 4) ^ REVV_BE(v1); \
  *(vec*)(op + d + 8) = *(vec*)(in + d + 8) ^ REVV_BE(v2); \
  *(vec*)(op + d + 12) = *(vec*)(in + d + 12) ^ REVV_BE(v3);

#define WRITE(op, d, v0, v1, v2, v3) \
  *(vec*)(op + d + 0) = REVV_BE(v0); \
  *(vec*)(op + d + 4) = REVV_BE(v1); \
  *(vec*)(op + d + 8) = REVV_BE(v2); \
  *(vec*)(op + d + 12) = REVV_BE(v3);

int crypto_stream_xor(uint8_t* out,
                      const uint8_t* in,
                      register size_t inlen,
                      const uint8_t* const restrict n,
                      const uint8_t* const restrict k)
/* Assumes all pointers are aligned properly for vector reads.
 * (dlg): nonce and key do not have to be aligned; they are copied
 * to aligned buffers on the stack. */
{
  assert(isaligned(out, 16) && "out not aligned to 16 bytes");
  assert(isaligned(in, 16) && "in not aligned to 16 bytes");
  assert(isaligned(n, 16) && "nonce not aligned");
  assert(isaligned(k, 16) && "key not aligned");
  size_t iters, i;
  uint32_t* op = (uint32_t*)out;
  uint32_t* ip = (uint32_t*)in;
  uint32_t* kp, *np;
  __attribute__((aligned(16))) uint32_t key[8], nonce[2];
  __attribute__((aligned(16)))
  uint32_t chacha_const[] = {0x61707865, 0x3320646E, 0x79622D32, 0x6B206574};
#if (__ARM_NEON__ || __SSE2__)
  memcpy(key, k, 8);
  memcpy(nonce, n, 8);
  kp = (uint32_t*)key;
  np = (uint32_t*)nonce;
#else
// TODO(dlg): check that this is still correct!
#warning-- This code has not been tested for correct AltiVec support.
  memcpy(key, k, 8);
  memcpy(nonce, n, 8);
  ((vec*)key)[0] = REVV_BE(((vec*)k)[0]);
  ((vec*)key)[1] = REVV_BE(((vec*)k)[1]);
  nonce[0] = REVW_BE(((uint32_t*)n)[0]);
  nonce[1] = REVW_BE(((uint32_t*)n)[1]);
  kp = (uint32_t*)key;
  np = (uint32_t*)nonce;
#endif
  vec s0 = *(vec*)chacha_const;
  vec s1 = ((vec*)kp)[0];
  vec s2 = ((vec*)kp)[1];

  vec s3 = NONCE(np);
  for (iters = 0; iters < inlen / (BPI * 64); iters++) {
    vec v0, v1, v2, v3, v4, v5, v6, v7;
    v4 = v0 = s0;
    v5 = v1 = s1;
    v6 = v2 = s2;
    v3 = s3;
    v7 = v3 + ONE;
#if VBPI > 2
    vec v8, v9, v10, v11;
    v8 = v4;
    v9 = v5;
    v10 = v6;
    v11 = v7 + ONE;
#endif
#if VBPI > 3
    vec v12, v13, v14, v15;
    v12 = v8;
    v13 = v9;
    v14 = v10;
    v15 = v11 + ONE;
#endif
#if GPR_TOO
    register uint32_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12,
        x13, x14, x15;
    x0 = chacha_const[0];
    x1 = chacha_const[1];
    x2 = chacha_const[2];
    x3 = chacha_const[3];
    x4 = kp[0];
    x5 = kp[1];
    x6 = kp[2];
    x7 = kp[3];
    x8 = kp[4];
    x9 = kp[5];
    x10 = kp[6];
    x11 = kp[7];
    // TODO(dlg): THIS IS WRONG!!!!!!!!!!
    x12 = BPI * iters + (BPI - 1);
    x13 = 0;
    x14 = np[0];
    x15 = np[1];
#endif
    for (i = CHACHA_RNDS / 2; i; i--) {
      DQROUND_VECTORS(v0, v1, v2, v3)
      DQROUND_VECTORS(v4, v5, v6, v7)
#if VBPI > 2
      DQROUND_VECTORS(v8, v9, v10, v11)
#endif
#if VBPI > 3
      DQROUND_VECTORS(v12, v13, v14, v15)
#endif
#if GPR_TOO
      QROUND_WORDS(x0, x4, x8, x12)
      QROUND_WORDS(x1, x5, x9, x13)
      QROUND_WORDS(x2, x6, x10, x14)
      QROUND_WORDS(x3, x7, x11, x15)
      QROUND_WORDS(x0, x5, x10, x15)
      QROUND_WORDS(x1, x6, x11, x12)
      QROUND_WORDS(x2, x7, x8, x13)
      QROUND_WORDS(x3, x4, x9, x14)
#endif
    }
    WRITE_XOR(ip, op, 0, v0 + s0, v1 + s1, v2 + s2, v3 + s3)
    s3 += ONE;
    WRITE_XOR(ip, op, 16, v4 + s0, v5 + s1, v6 + s2, v7 + s3)
    s3 += ONE;
#if VBPI > 2
    WRITE_XOR(ip, op, 32, v8 + s0, v9 + s1, v10 + s2, v11 + s3)
    s3 += ONE;
#endif
#if VBPI > 3
    WRITE_XOR(ip, op, 48, v12 + s0, v13 + s1, v14 + s2, v15 + s3)
    s3 += ONE;
#endif
    ip += VBPI * 16;
    op += VBPI * 16;
#if GPR_TOO
    op[0] = REVW_BE(REVW_BE(ip[0]) ^ (x0 + chacha_const[0]));
    op[1] = REVW_BE(REVW_BE(ip[1]) ^ (x1 + chacha_const[1]));
    op[2] = REVW_BE(REVW_BE(ip[2]) ^ (x2 + chacha_const[2]));
    op[3] = REVW_BE(REVW_BE(ip[3]) ^ (x3 + chacha_const[3]));
    op[4] = REVW_BE(REVW_BE(ip[4]) ^ (x4 + kp[0]));
    op[5] = REVW_BE(REVW_BE(ip[5]) ^ (x5 + kp[1]));
    op[6] = REVW_BE(REVW_BE(ip[6]) ^ (x6 + kp[2]));
    op[7] = REVW_BE(REVW_BE(ip[7]) ^ (x7 + kp[3]));
    op[8] = REVW_BE(REVW_BE(ip[8]) ^ (x8 + kp[4]));
    op[9] = REVW_BE(REVW_BE(ip[9]) ^ (x9 + kp[5]));
    op[10] = REVW_BE(REVW_BE(ip[10]) ^ (x10 + kp[6]));
    op[11] = REVW_BE(REVW_BE(ip[11]) ^ (x11 + kp[7]));
    op[12] = REVW_BE(REVW_BE(ip[12]) ^ (x12 + BPI * iters + (BPI - 1)));
    op[13] = REVW_BE(REVW_BE(ip[13]) ^ (x13));
    op[14] = REVW_BE(REVW_BE(ip[14]) ^ (x14 + np[0]));
    op[15] = REVW_BE(REVW_BE(ip[15]) ^ (x15 + np[1]));
    s3 += ONE;
    ip += 16;
    op += 16;
#endif
  }
  for (iters = inlen % (BPI * 64) / 64; iters != 0; iters--) {
    vec v0 = s0, v1 = s1, v2 = s2, v3 = s3;
    for (i = CHACHA_RNDS / 2; i; i--) {
      DQROUND_VECTORS(v0, v1, v2, v3)
    }
    WRITE_XOR(ip, op, 0, v0 + s0, v1 + s1, v2 + s2, v3 + s3)
    s3 += ONE;
    ip += 16;
    op += 16;
  }
  inlen = inlen % 64;
  if (inlen) {
    __attribute__((aligned(16))) vec buf[4];
    vec v0, v1, v2, v3;
    v0 = s0;
    v1 = s1;
    v2 = s2;
    v3 = s3;
    for (i = CHACHA_RNDS / 2; i; i--) {
      DQROUND_VECTORS(v0, v1, v2, v3)
    }
    if (inlen >= 16) {
      *(vec*)(op + 0) = *(vec*)(ip + 0) ^ REVV_BE(v0 + s0);
      if (inlen >= 32) {
        *(vec*)(op + 4) = *(vec*)(ip + 4) ^ REVV_BE(v1 + s1);
        if (inlen >= 48) {
          *(vec*)(op + 8) = *(vec*)(ip + 8) ^ REVV_BE(v2 + s2);
          buf[3] = REVV_BE(v3 + s3);
        } else {
          buf[2] = REVV_BE(v2 + s2);
        }
      } else {
        buf[1] = REVV_BE(v1 + s1);
      }
    } else
      buf[0] = REVV_BE(v0 + s0);
    for (i = inlen & ~15; i < inlen; i++)
      ((uint8_t*)op)[i] = ((uint8_t*)ip)[i] ^ ((uint8_t*)buf)[i];
  }
  // memwipe(key, 8);
  // memwipe(nonce, 2);
  return 0;
}
