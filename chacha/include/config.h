#ifdef __ARM_NEON__
#define CHACHA_KROVETZ
#elif __ALTIVEC__
#define CHACHA_KROVETZ
#elif __SSE2__
#define CHACHA_KROVETZ
#else
#define CHACHA_REF
#endif
