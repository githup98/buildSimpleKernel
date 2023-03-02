section .asm

global insb
global insw
global outb
global outw

insb:
	push ebp      ; backup stack base pointer
	mov ebp, esp

	xor eax, eax  ; due to eax alway return the value

	mov edx, [ebp+8]

	in al, dx

	pop ebp
	ret

insw:
	push ebp      ; backup stack base pointer
	mov ebp, esp

	xor eax, eax  ; due to eax alway return the value

	mov edx, [ebp+8]

	in ax, dx

	pop ebp
	ret


outb:
	push ebp
	mov ebp, esp
	
	xor eax, eax
	xor edx, edx
	mov edx, [ebp+8]	; for port - first argument
	mov eax, [ebp+12]
	out	dx, al   ; out 1 byte in al to port which address in dx

	pop ebp
	ret

outw:
	push ebp
	mov ebp, esp
	
	xor eax, eax
	xor edx, edx
	mov edx, [ebp+8]
	mov eax, [ebp+12]
	out	dx, ax   ; out 2 byte in al to port which address in dx

	pop ebp
	ret
