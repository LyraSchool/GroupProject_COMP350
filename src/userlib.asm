;userlib.asm
;Margaret Black, 2019

;userlib.asm contains assembly functions for user programs such as shell

	.global _syscall
	.global _enableInterrupts

;void syscall (int AX, int BX, int CX, int DX)
_syscall:
	push bp
	mov bp,sp
	mov ax,[bp+4]	;get the parameters AX, BX, CX, and DX
	mov bx,[bp+6]
	mov cx,[bp+8]
	mov dx,[bp+10]
	int #0x21
	pop bp
	ret

;void enableInterrupts()
;call at the beginning of programs.  allows timer preemption
_enableInterrupts:
	sti
	ret
