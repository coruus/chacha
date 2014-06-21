[bits 64]
[CPU intelnop]

%define arg1 rdi
%define arg2 rsi



align 32
section .data

incposk: dq 0x0000000000000002
         dq 0x0000000000000003


; load a stream position, and shuffle pos+0, pos+1,
; pos+2, pos+3 into two xmm registers
%macro incpos 3
  ; clobbers rax and rdx
  %define position0 %1  ; xmm register
  %define position1 %2  ; xmm register
  %define input     %3  ; valid input for mov
  %define rpos0 rax
  %define rpos1 rdx
  %define epos0 eax
  %define epos1 edx
  mov        rpos0, input
  mov        rpos1, rpos0
  add        rpos1, 1
  movq       position1, rpos0
  movlhps    position1, position1
  paddq      position1, [incposk wrt rip]
  pshufd     position0, position1,  0x80

  pinsrd     position0, epos0, 0x0
  pinsrd     position0, epos1, 0x1

  shr        rpos0, 32
  shr        rpos1, 32

  psrlq      position1,  32
  pshufd     position1, position1,  0x80
  pinsrd     position1, epos0, 0x0
  pinsrd     position1, epos1, 0x1
%endmacro


align 32
section .text
global bpos
bpos:

  incpos  xmm0, xmm1, rdi

  movdqu     [arg2], xmm0
  movdqu     [arg2+16], xmm1
