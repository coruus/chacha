#ifndef CHACHA_UTILS_H
#define CHACHA_UTILS_H
#include <assert.h>

#ifndef DEBUG
#define __unreachable() __builtin_unreachable()
#else
#define __unreachable() assert(0 && "unreachable")
#endif

#endif
