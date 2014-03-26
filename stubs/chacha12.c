#define CHACHA_RNDS 12
#define crypto_stream_xor chacha12_xor
#include "config.h"

#ifdef CHACHA_KROVETZ
#include "krovetz/chacha-krovetz.c"
#else
#include "chacha-ref.c"
#endif  // CHACHA_KROVETZ