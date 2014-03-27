#include <stdint.h>
#include <stdlib.h>

int chacha8_xor(uint8_t* out,
                const uint8_t* in,
                size_t inlen,
                const uint8_t* n,
                const uint8_t* k);
int chacha12_xor(uint8_t* out,
                 const uint8_t* in,
                 size_t inlen,
                 const uint8_t* n,
                 const uint8_t* k);
int chacha20_xor(uint8_t* out,
                 const uint8_t* in,
                 size_t inlen,
                 const uint8_t* n,
                 const uint8_t* k);
int chacha_xor(uint8_t* out,
               const uint8_t* in,
               size_t inlen,
               const uint8_t* nonce,
               const uint8_t* key,
               const size_t rounds);
