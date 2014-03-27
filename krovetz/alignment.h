#ifndef ALIGNMENT_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static inline bool isaligned(const void* const ptr, size_t alignment);

static inline bool isaligned(const void* const ptr, size_t alignment) {
  return ((uintptr_t)ptr % alignment) == 0;
}

static inline int realignto(void** const restrict realigned,
                            const void* const restrict ptr,
                            register const size_t alignment,
                            register const size_t size) {
  register size_t misalignment = ((uintptr_t)ptr) % alignment;
  if (misalignment != 0) {
    int err = posix_memalign(realigned, alignment, size);
    if (err != 0) {
      return err;
    }
    memcpy(realigned, ptr, size);
  } else {
    *realigned = ptr;
    return 0;
  }
}

#define DEFAULT_ALIGNMENT 16
#define realign(REALIGNED, PTR, SIZE) \
  realignto(REALIGNED, PTR, DEFAULT_ALIGNMENT, SIZE)

#endif
