#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

void broadcastpos(uint64_t position, uint32_t dest[2][4]) {
  for (int i = 0; i < 4; i++) {
    dest[0][i] = (uint32_t)((position + i) & (0xffffffff));
  }
  for (int i = 0; i < 4; i++) {
    dest[1][i] = (uint32_t)((position + i) >> 32);
  }
}

extern void bpos(uint64_t position, uint32_t* dest);

int main(void) {
  uint32_t pos[2][4] = {0};
  uint64_t K = 5;  // 0xfffffffffffffffeULL;
  broadcastpos(K, pos);  // 0x00000000ffffffffULL);
  for (int i = 0; i < 4; i++) {
    printf("%u ", pos[0][i]);
  }
  printf("\n");
  for (int i = 0; i < 4; i++) {
    printf("%u ", pos[1][i]);
  }
  printf("\n");
  uint32_t* p = malloc(2 * 4 * 4);
  bpos(K, p);
  for (int i = 0; i < 4; i++) {
    printf("%u ", p[0 + i]);
  }
  printf("\n");
  for (int i = 0; i < 4; i++) {
    printf("%u ", p[4 + i]);
  }
  printf("\n");

  return 0;
}
