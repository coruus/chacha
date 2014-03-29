#ifndef PRINT_STATE_H
#define PRINT_STATE_H
#include <stdio.h>

static void inline print_state(uint32_t state[16]);
static void inline print_state(uint32_t state[16]) {
  printf("\nchacha_state");
  for (size_t i = 0; i < 16; i++) {
    if ((i % 4) == 0) {
        printf("\n  ");
     } else {
        printf(" ");
     }
    printf("%08x", state[i]);
  }
  printf("\n");
}


#endif
