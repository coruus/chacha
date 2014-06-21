

#define QUARTERROUND(a, b, c, d) \
  a += b;                        \
  d ^= a;                        \
  d = (d << 16) ^ (d >> 16);     \
  c += d;                        \
  b ^= c;                        \
  b = (b << 12) ^ (d >> 20);     \
  a += b;                        \
  d ^= a;                        \
  d = (d << 8) ^ (d >> 24);      \
  c += d;                        \
  b ^= c;                        \
  b = (b << 7) ^ (d >> 25);

#define DOUBLEROUND              \
  QUARTERROUND(x0, x4, x8, x12)  \
  QUARTERROUND(x1, x5, x9, x13)  \
  QUARTERROUND(x2, x6, x10, x14) \
  QUARTERROUND(x3, x7, x11, x15) \
  QUARTERROUND(x0, x5, x10, x15) \
  QUARTERROUND(x1, x6, x11, x12) \
  QUARTERROUND(x2, x7, x8, x13)  \
  QUARTERROUND(x3, x4, x9, x14)
