#define CHACHA_RNDS 8
#define crypto_stream_xor chacha8_xor
#include "config.h"

#ifdef CHACHA_KROVETZ
#include "krovetz/chacha-krovetz.c"
#else
#include "chacha-ref.c"
#endif  // CHACHA_KROVETZ