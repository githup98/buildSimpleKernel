section .asm
[BITS 32]
global pagingLoadDirectory
global enablePage

pagingLoadDirectory:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8] ; first argument
	mov cr3, eax  ;; see osdev paing (enable paging)
	pop ebp
	ret
		


enablePage:
	push ebp
	mov ebp, esp
	xor eax, eax
	mov eax, cr0
	;or eax, 0x80000001  ; enable PG, PE bits in cr0 reg
	or eax, 0x80000000  ; enable Page, not enable secure page
	mov cr0, eax
	pop ebp
	ret

