from __future__ import division, print_function

MOV = "movdqa   xmm{xmm}, [x+{offset}]"

STATIC = [(0, 1),
          (4, 2),
          (8, 3),
          (12, 4),
          (1, 5),
          (5, 6),
          (13, 8),
          (2, 9),
          (6, 10),
          (10, 11),
          (14, 12),
          (3, 13),
          (11, 14),
          (15, 15)]


