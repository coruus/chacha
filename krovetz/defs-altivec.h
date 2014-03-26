#include <altivec.h>
#define GPR_TOO 1
#define VBPI 3
#define ONE ((vec) {1, 0, 0, 0})
#define NONCE(p) vec_sro(*(vec*)p, (vector char)(vec) {0, 0, 0, 8 * 8})
#define REVW_BE(x) __builtin_bswap32(x)
#define REVV_BE(x) \
  vec_perm(        \
      x,           \
      x,           \
      (vector char) {3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12})
#define ROTV1(x) \
  vec_perm(      \
      x,         \
      x,         \
      (vector char) {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3})
#define ROTV2(x) \
  vec_perm(      \
      x,         \
      x,         \
      (vector char) {8, 9, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7})
#define ROTV3(x) \
  vec_perm(      \
      x,         \
      x,         \
      (vector char) {12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11})
#define ROTW7(x) vec_rl(x, vec_splat_u32(7))
#define ROTW8(x) vec_rl(x, vec_splat_u32(8))
#define ROTW12(x) vec_rl(x, vec_splat_u32(12))
#define ROTW16(x) vec_rl(x, vec_splat_u32(-16)) /* trick to get 16 */
