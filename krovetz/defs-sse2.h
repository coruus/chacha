#include <emmintrin.h>

#define GPR_TOO 0

#if __clang__

#define VBPI 4

#else

#define VBPI 3

#endif  // __clang__

#define ONE (vec) _mm_set_epi32(0, 0, 0, 1)
#define NONCE(p) (vec) _mm_slli_si128(_mm_loadl_epi64((__m128i*)(p)), 8)
#define ROTV1(x) (vec) _mm_shuffle_epi32((__m128i)x, _MM_SHUFFLE(0, 3, 2, 1))
#define ROTV2(x) (vec) _mm_shuffle_epi32((__m128i)x, _MM_SHUFFLE(1, 0, 3, 2))
#define ROTV3(x) (vec) _mm_shuffle_epi32((__m128i)x, _MM_SHUFFLE(2, 1, 0, 3))
#define ROTW7(x) \
  (vec)(_mm_slli_epi32((__m128i)x, 7) ^ _mm_srli_epi32((__m128i)x, 25))
#define ROTW12(x) \
  (vec)(_mm_slli_epi32((__m128i)x, 12) ^ _mm_srli_epi32((__m128i)x, 20))

#if __SSSE3__

#include <tmmintrin.h>
#define ROTW8(x)          \
  (vec) _mm_shuffle_epi8( \
      (__m128i)x,         \
      _mm_set_epi8(14, 13, 12, 15, 10, 9, 8, 11, 6, 5, 4, 7, 2, 1, 0, 3))
#define ROTW16(x)         \
  (vec) _mm_shuffle_epi8( \
      (__m128i)x,         \
      _mm_set_epi8(13, 12, 15, 14, 9, 8, 11, 10, 5, 4, 7, 6, 1, 0, 3, 2))

#else

#define ROTW8(x) \
  (vec)(_mm_slli_epi32((__m128i)x, 8) ^ _mm_srli_epi32((__m128i)x, 24))
#define ROTW16(x) \
  (vec)(_mm_slli_epi32((__m128i)x, 16) ^ _mm_srli_epi32((__m128i)x, 16))

#endif  // __SSSE3__
