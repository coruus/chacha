[bits 64]
[CPU intelnop]

section .text

%define dest rdi

%macro store4 1
	movdqu [%1 + 0*16],xmm0
	movdqu [%1 + 1*16],xmm1
	movdqu [%1 + 2*16],xmm2
	movdqu [%1 + 3*16],xmm3
%endmacro

align 16
global memclear64bytes
memclear64bytes:
  pxor   xmm0, xmm0
  pxor   xmm1, xmm1
  pxor   xmm2, xmm2
  pxor   xmm3, xmm3
  store4(dest)
  ret
