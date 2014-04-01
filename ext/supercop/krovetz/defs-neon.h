#include <arm_neon.h>

#define GPR_TOO 1
#define VBPI 2
#define ONE (vec) vsetq_lane_u32(1, vdupq_n_u32(0), 0)
#define NONCE(p) (vec) vcombine_u32(vdup_n_u32(0), vcreate_u32(*(uint64_t*)p))
#define ROTV1(x) (vec) vextq_u32((uint32x4_t)x, (uint32x4_t)x, 1)
#define ROTV2(x) (vec) vextq_u32((uint32x4_t)x, (uint32x4_t)x, 2)
#define ROTV3(x) (vec) vextq_u32((uint32x4_t)x, (uint32x4_t)x, 3)
#define ROTW16(x) (vec) vrev32q_u16((uint16x8_t)x)

#if __clang__

#define ROTW7(x) (x << ((vec) {7, 7, 7, 7})) ^ (x >> ((vec) {25, 25, 25, 25}))
#define ROTW8(x) (x << ((vec) {8, 8, 8, 8})) ^ (x >> ((vec) {24, 24, 24, 24}))
#define ROTW12(x) \
  (x << ((vec) {12, 12, 12, 12})) ^ (x >> ((vec) {20, 20, 20, 20}))

#else

#define ROTW7(x) \
  (vec) vsriq_n_u32(vshlq_n_u32((uint32x4_t)x, 7), (uint32x4_t)x, 25)
#define ROTW8(x) \
  (vec) vsriq_n_u32(vshlq_n_u32((uint32x4_t)x, 8), (uint32x4_t)x, 24)
#define ROTW12(x) \
  (vec) vsriq_n_u32(vshlq_n_u32((uint32x4_t)x, 12), (uint32x4_t)x, 20)

#endif  // __clang__
