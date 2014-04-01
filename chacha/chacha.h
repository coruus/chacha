#include <stdint.h>
#include <stdlib.h>

/* The easy start-from-zero ChaCha API. */
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

/* The more flexible chacha_state API; supports starting from
 * arbitrary positions in the ChaCha stream. Useful for applications
 * that can benefit from coarse-scale parallelization (i.e., are
 * enciphering large quantities of data).
 */
// Initialize a ChaCha state.
int chacha_state_init(uint32_t chacha_state[16],
                      register const uint8_t* const restrict key,
                      register const uint8_t* const restrict nonce,
                      register const size_t stream_position,
                      register const size_t keybitlen);
// Re-set the stream position and nonce; does not affect the nonce
// if NULL is passed as a nonce.
int chacha_state_reset(uint32_t chacha_state[16],
                       register const uint8_t* const restrict nonce,
                       register const size_t stream_position);

int chacha_state_chacha8(uint32_t chacha_state[16],
                         uint8_t* in,
                         const uint8_t* out,
                         register const size_t oplen);
int chacha_state_chacha12(uint32_t chacha_state[16],
                          uint8_t* in,
                          const uint8_t* out,
                          register const size_t oplen);
int chacha_state_chacha20(uint32_t chacha_state[16],
                          uint8_t* in,
                          const uint8_t* out,
                          register const size_t oplen);
