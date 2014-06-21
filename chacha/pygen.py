


def quarterround(a, b, c, d):
  return \
[add(a, a, b),
 xor(d, d, a),
 rol(d, 16),
 add(c, c, d),
 xor(b, b, c),
 rol(b, 12),
 add(a, a, b),
 xor(d, d, a),
 rol(d, 8),
 add(c, c, d),
 xor(b, b, c),
 rol(b, 7)]

