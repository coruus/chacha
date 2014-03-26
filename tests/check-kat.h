#include <stdint.h>
#include <stdlib.h>

int check_kat(const uint8_t key[32],
              const uint8_t nonce[8],
              const uint8_t* expected,
              register const size_t len,
              register const size_t rounds,
              const char* const restrict name);
