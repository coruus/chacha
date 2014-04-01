[bits 64]
[CPU intelnop]

section .text

align 16
global clearregs_avx
clearregs_sse:
  pxor   xmm0, xmm0
  pxor   xmm1, xmm1
  pxor   xmm2, xmm2
  pxor   xmm3, xmm3
  pxor   xmm4, xmm4
  pxor   xmm5, xmm5
  pxor   xmm6, xmm6
  pxor   xmm7, xmm7
  ret

align 32
global clearregs_avx
clearregs_avx:
  vpxor   ymm0, ymm0
  vpxor   ymm1, ymm1
  vpxor   ymm2, ymm2
  vpxor   ymm3, ymm3
  vpxor   ymm4, ymm4
  vpxor   ymm5, ymm5
  vpxor   ymm6, ymm6
  vpxor   ymm7, ymm7
;  vpxor   ymm8, xmm8
;  vpxor   ymm9, xmm9
;  vpxor   ymm10, xmm10
;  vpxor   ymm11, xmm11
;  vpxor   ymm12, xmm12
;  vpxor   ymm13, xmm13
;  vpxor   ymm14, xmm14
;  vpxor   ymm15, xmm15
;  vpxor   ymm16, xmm16
;  vpxor   ymm17, xmm17
;  vpxor   ymm18, xmm18
;  vpxor   ymm19, xmm19
;  vpxor   ymm20, ymm20
;  vpxor   ymm21, ymm21
;  vpxor   ymm22, ymm22
;  vpxor   ymm23, ymm23
;  vpxor   ymm24, ymm24
;  vpxor   ymm25, ymm25
;  vpxor   ymm26, ymm26
;  vpxor   ymm27, ymm27
;  vpxor   ymm28, ymm28
;  vpxor   ymm29, ymm29
;  vpxor   ymm30, ymm30
;  vpxor   ymm31, ymm31
;  vpxor   ymm32, ymm32
;  vpxor   ymm33, ymm33
  ret
