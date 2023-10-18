;kernel.asm
;Margaret Black, 2007

;kernel.asm contains assembly functions that you can use in your kernel

        .global _putInMemory
        .global _interrupt
        .global _makeInterrupt21
;       .extern _handleInterrupt21

;void putInMemory (int segment, int address, char character)
_putInMemory:
        push bp
        mov bp,sp
        push ds
        mov ax,[bp+4]
        mov si,[bp+6]
        mov cl,[bp+8]
        mov ds,ax
        mov [si],cl
        pop ds
        pop bp
        ret
