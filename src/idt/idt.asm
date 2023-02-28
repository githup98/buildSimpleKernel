section .asm

global idtLoad
global int21h
extern int21hHandler
global noInterrupt
extern noInterruptHandler


idtLoad:
	;; interact with stack point on stack frame	
	push ebp ; store previous stack base pointeri of stack frame
	mov ebp, esp;
	;;
	mov ebx, [ebp+8]  ; get first argument
	lidt [ebx]
	pop ebp ; restore stack base pointer
	ret

;; interrupt handler for interrupt 1 (keyboard press)
int21h:
	cli
	pushad    ; push all data registers, base register
	call int21hHandler
	popad
	sti
	iret

noInterrupt:
	cli
	pushad    ; push all data registers, base register
	call noInterruptHandler
	popad
	sti
	iret
