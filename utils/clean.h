#ifndef CLEAN_H
void memclear64bytes(void* dest);
#define clearstate memclear64bytes
#ifdef __AVX__
void clearregs_avx(void);
#define clearregs clearregs_avx
#else
void clearregs_sse(void);
#define clearregs clearregs_sse
#endif // __AVX__
#endif // CLEAN_H
