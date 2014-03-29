[bits 64]
[CPU intelnop]

align 16
global do_rdtsc
do_rdtsc:

	rdtsc
	shl rdx, 32
	or  rax, rdx
	ret	0
