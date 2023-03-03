; visit website : wiki.osdev.org/FAT
;; this line is not for almost
;ORG 0x7c00   ; jump to this address
			; the BIOS load bootloader into this address

ORG 0x7c00; for testing protected mode
;ORG 0;


CODE_SEG equ gdt_code - gdt_start  ; cal address of this segment
DATA_SEG equ gdt_data - gdt_start


BITS 16   ;; tell to assembler that it use 16-bit architecture
		  ;; due to this program is a boot looader program, it run in real mode
		  ;; only 16-bit mode, access 1 MB RAM only

;;; this snipple code foe suring boot sucessfully in all bios (visit wiki.osdev.org/FAT

;; interrupt is a subroutine, 


jmp short start
nop

; FAT Header (Fast allocation table - FAT)
OEMIdentifier         db 'TRUCDOOS' ;; 8 bytes
BytesPerSector        dw 0x200      ;; 512 bytes
SectorsPerCluster     db 0x80
ReservedSectors       dw 200        ;; store all our kernel
FATCopies             db 0x02       ;; FAT and one backup
RootDirEntries        dw 0x40
NumSectors            dw 0x00
MediaType             db 0xF8
SectorsPerFAT         dw 0x100
SectorsPerTrack       dw 0x20
NumberOfHeads         dw 0x40
HiddenSectors         dd 0x00
SectorsBig            dd 0x773594

;Extend head (BPB 4.0) //BIOS parameter block

DriveNumber           db 0x80
WinNTBit              db 0x00
Signature             db 0x29
VolumeID              dd 0xD105
VolumeIDString        db 'TRUCDO BOOT' ;;11 bytes  
SystemIDString        db 'FAT16   '    ;; 8 bytes


start:

	;jmp 0x7c0:step2
	jmp 0:step2   ; for testing protected mode

;;;;;;;;;; end snipple code
;handleZero:
;	mov ah, 0eh
;	mov al, "P"
;	mov bx, 0x00
;	int 0x10    ; interrupt for dump char to screen
;	iret        ; return from interrupt



step2:

	cli  ; clear interrupt
	
	; data segment: 0x7c
	;mov ax, 0x7c0
	mov ax, 0   ; for testing entry protected mode
	mov ds, ax	; data segment
	mov es, ax  ; extra segment
	
	; stack segment: 0
	;mov ax, 0  ; due to testing entry protected mode
	mov ss, ax
	mov sp, 0x7c00


	sti  ; enable interrupt


	;; test read a sector to buffer
	;mov ah, 2 ; 	  read sector command
	;mov al, 1 ; 	  number sector to read
	;mov cl, 2 ;     read 2-snd sector
	;mov ch, 0
	;mov dh, 0
	;mov bx, buffer   ; point bx to buffer (buffer lable)
	;
	;int 0x13 ; interrupt number


	;jc error   ; if reading the sector has error, CF flag will be set
	;mov si, buffer
	;call print

	;jmp $  ; this line look like a sys_ext, exit program

	;error:
	;
	;mov si, errorMsg
	;call print     ; dump read sector
	;
	;jmp $
	;; end of testing to read sector



	; register an interrupt
	
	; due to offset or segment in interrupt are 2 byte
	
	;; test table interrupt

	;mov word[ss:0x00], handleZero ; change to stack segment
	;mov word[ss:0x02], 0x7c0;     ; change to back a data segment

	;int 0
	
	
	
	;; cause an exception
	;; an exception is interrupt also, div-by-zero has interrupt number is 0
	;;	--> no need to call int 0, the handleZero will still be called
	;mov ax, 0x00
	;div ax

	;; end of testing of table interrupt


	;; testing dump message to screen
	
	;mov si, message
	;call print
	;; end of testing
	
	;jmp $  ; this line look like a sys_ext

	
	;; testing entry protected mode, we need to configure address for code segment
	;; data segmetn, extra segment ... before entry protected mode

.load_protected:  ; local lable
	cli 
	lgdt [gdt_descriptor]  ; load GDT register with start address of 
						   ; global dessriptor table
	mov eax, cr0
	or al, 1     ; bit enable protected mode is bit 0 of cr0 register
	mov cr0, eax

	jmp CODE_SEG:load32

	;PModeMain:
	; load DS, ES, FS, GS, SS, ESP


gdt_start:

gdt_null: ; due to in protected mode, code segment must be at 0x8h
	dd 0x0
	dd 0x0
;offset 0x8
gdt_code:  	   ; CS shoulde point to this
	dw 0xffff ; access fully 4G RAM (segment limit, fisrt 0-15 bits)
	dw 0       ; Base first 0-15 bits
	db 0       ; base second 16-23 bits
	db 0x9a    ; Access byte refer to osdev.org/Global_descriptor_Table for details
	db 11001111b ;high 4-bits flags, low 4-bits for limit 16-19 bits
	db 0       ; Base from 24-31 bits
	;; end of testing entry protected mode

; offset 0x10
gdt_data:  	   ; DS, SS,  ES, GS, FS shoulde point to this
	dw 0xffff ; access fully 4G RAM (segment limit, fisrt 0-15 bits)
	dw 0       ; Base first 0-15 bits
	db 0       ; base second 16-23 bits
	db 0x92    ; Access byte refer to osdev.org/Global_descriptor_Table for details
	db 11001111b ;high 4-bits flags, low 4-bits for limit 16-19 bits
	db 0       ; Base from 24-31 bits
	;; end of testing entry protected mode

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start-1
	dd gdt_start ; offset of gdt_start label


	[BITS 32]
	load32:			;load kernel to memory
		mov eax, 1  ; sector start read (skip sector 0, it is bootloader)
		mov ecx, 100 ; number secttor wanna read
		mov edi, 0x0100000 ; 1M (size want to load into)
		call ata_lba_read
		jmp CODE_SEG:0x0100000 ; where the kernel is loaded into

ata_lba_read:
	mov ebx, eax ; backup lba
	;; Send the highest 8-bits of lba to hard disk controller
	shr eax, 24
	or eax, 0xE0  ; select the master drive
	mov dx, 0x1F6
	out dx, al
	;; Finish sending


	;; send total sector to read
	mov eax, ecx
	mov dx, 0x1F2
	out dx, al
	;; Finish sending total sector to read

	;; send more bits of LBA
	mov eax, ebx  ; restore the backup lba
	mov dx, 0x1F3
	out dx, al
	;; finish sending more bits of LBA

	 
	;; send more bits of LBA
	mov eax, ebx  ; restore the backup lba
	mov dx, 0x1F4
	shr eax, 8
	out dx, al
	;; finish sending more bits of LBA

	;; send 16 bits of LBA
	mov eax, ebx  ; restore the backup lba
	mov dx, 0x1F5
	shr eax, 16
	out dx, al
	;; finish sending 16 bits of LBA

	mov dx, 0x1f7
	mov al, 0x20	; read sector command, 0x30 write sector with retry command
	out dx, al

.next_sector: ;reading all sector to memory
	push ecx
.try_again:  ; check if we need to read again
	mov dx, 0x1f7
	in al, dx
	test al, 8
	jz .try_again
	; we need read 256 words at a time, it's mean 1 sector (512 bytes)
	mov ecx, 256
	mov dx,0x1f0
	rep insw
	pop ecx   ; restore ecx from stack (push ecx)
	loop .next_sector
	ret
;	
;	;; Enable A20 line
;
;	in al, 0x92
;	or al, 2
;	out 0x92, al	
;	;;
;
;	jmp $ ; do nothing (same as exit system call)

;print:
;	mov bx,0
;.loop:			; it means it is the local label which not included in object
;				; file's symbol table
;	lodsb   ; load data from si to al register
;
;	cmp al, 0
;	je .done
;	call printChar
;	jmp .loop 
;
;
;.done:
;	ret
;
;printChar:
;	mov ah, 0eh		; this value from ctyme.com webiste (type Ralf Brown's interrupt)
;	int 0x10
;	ret
;

;errorMsg: db 'Failed to read a sector', 0



;message: db 'Hello, world!', 0
;message: db 'This is the fisrt bootloader by TrucDo, Welcome to my computer', 0

times 510-($ - $$) db 0    ;if the code is not code 510 byte, this line will be
							; fill 0 from end of code to 509th byte
dw 0xAA55		;; this value is boot signature, when bios found that in
				;; first sector, it will be load on sector to 7c00 address (it means
				;; bootloader must be in this range of the first sector 0 --> 0x55AA)

;buffer:
