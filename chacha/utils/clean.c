#include "utils/clean.h"
#include <string.h>

void memclear64bytes(void* dest) {
  memset(dest, 0, 64);
}

// Clear registers is a no-op in the C-only version
void clearregs(void) {
  return;
}
