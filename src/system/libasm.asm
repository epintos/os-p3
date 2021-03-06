GLOBAL  _read_msw,_lidt
GLOBAL  _int_80_hand
GLOBAL  _int_09_hand
GLOBAL  _int_08_hand
GLOBAL  _int_0_hand
GLOBAL  _int_05_hand
GLOBAL  _int_06_hand
GLOBAL	_int_B_hand
GLOBAL	_int_C_hand
GLOBAL	_int_D_hand
GLOBAL  _int_E_hand
GLOBAL  _call_int_08
GLOBAL  _restart_PC
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL _wrapper_int_80
GLOBAL _screenCursorPos
GLOBAL  CPU_Speed
GLOBAL	_Halt
GLOBAL	_out
GLOBAL	_inw
GLOBAL  _outw
GLOBAL	_in
GLOBAL 	_inb
GLOBAL	_outb
GLOBAL  _activatePaging
GLOBAL	_setPageDir
GLOBAL	_printASM
GLOBAL  _readCR2
GLOBAL	_getESP

EXTERN  int_08
EXTERN  int_09
EXTERN  service_Read
EXTERN  service_Write
EXTERN 	int_80
EXTERN	int_81
EXTERN _saveContext
EXTERN _getNewContext
EXTERN _GetTemporaryESP
EXTERN snp
EXTERN div0
EXTERN bounds
EXTERN invop
EXTERN ssf
EXTERN gpf



SECTION .text

_Halt:
	hlt
	ret


_Cli:
	cli			; Cleans interruption flags
	ret

_Sti:

	sti			; Enables interruption flags
	ret

_mascaraPIC1:			; Writes PIC 1 mask
		push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = 16 bits mask
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Writes PIC 2 mask
		push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = 16 bits mask
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Gets the Machine Status Word
        retn


_lidt:				; Loads IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = IDTR pointer
		rol	ebx,16		    	
		lidt    [ds: ebx]          ; Loads IDTR
        pop     ebx
        pop     ebp
        retn

_call_int_08:
	push ebp
	mov ebp,esp
	int 20h
	mov	esp,ebp
	pop	ebp
	ret

_int_08_hand:				; INT 8 Handler(Timer Tick)
	cli
	pushad
		mov eax, esp
		push eax
			call _saveContext
		pop ecx
		call _GetTemporaryESP
		mov esp, eax
		mov eax, 0
		call _getNewContext
		mov esp,eax
		;call _debug;
	popad
	mov al,20h			; Envio de EOI generico al PIC
	out 20h,al

	sti

	iret
		
_int_09_hand:				; INT 9 Handler (Keyboard)
			push ebp 		; Does StackFrame
			mov ebp,esp
			pusha
			
			mov eax,00h

			in al, 60h 		; Reads ScanCode from Keyboard
			mov ah, 00h	

			push eax;		; Parameter for C function

			call int_09		; Calls C function

			pop eax;

			mov	al,20h		; EOI to PIC
			out	20h,al
			
			popa
			mov	esp,ebp		; Breaks up StackFrame
			pop	ebp
			
			iret

_wrapper_int_80:
		push ebp
		mov	ebp,esp
		pusha
		
		mov	ecx,00h
	
		; Loads parameters from C
		mov	ecx,[ebp+8]		; Loads the operation
		;mov	ecx,[ebp+12]	; Loads data
		;mov	ebx,[ebp+16]	; Loads the file descriptor
		
		int 80h
		
		popa
		mov	esp,ebp
		pop	ebp
		ret			
						
_int_80_hand:
			cli				; INT 80 Handler (System Call)
			push ebp		; Does StackFrame
			mov ebp, esp	
			pusha	
			
			
			; Push parameters to int_80 in C
			;push ebx
			;push ecx
			push ecx
			
			call int_80

			pop ecx
			;pop ecx
			;pop ebx

			popa
			mov	esp,ebp			; Breaks up StackFrame
			pop	ebp

			sti
			iret			

_int_E_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call snp		; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			;jmp $

			iret

_int_0_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call div0		; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			jmp $

			iret

_int_05_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call bounds		; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			jmp $

			iret

_int_06_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call invop	; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			jmp $
			iret
_int_B_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call snp		; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			jmp $
			iret

_int_C_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call ssf	; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			jmp $
			iret

_int_D_hand:
			push ds
			push es				; Se salvan los registros
			pusha				; Carga de DS y ES con el valor del selector
			pushf
			mov eax, 666666h
			mov ax, 10h			; a utilizar.
			mov ds, ax
			mov es, ax
			call gpf	; Calls C function
			mov	al,20h		; EOI to PIC
			out	20h,al
			popf
			popa
			pop ax
			pop ax
			jmp $
			iret

; Warm boot, taken from:
; http://www.uv.tietgen.dk/staff/mlha/pc/prog/asm/index.htm
_restart_PC:	
			mov ax,40h
			mov ds,ax
			mov ax, 1234h ; Warm boot flag, bypass Memory Test
			mov [72h], ax
			jmp 0FFFFh:0000h ;At FFFF:0000 is the intruction
								; JMP F000:E05B

				
_screenCursorPos:
			push ebp
			mov ebp, esp
			mov dx, [ebp+8]
			mov al, [ebp+12]
			out dx, al
			mov esp, ebp
			pop ebp
			ret		
			

CPU_Speed:
			push ebp
			mov ebp, esp
			
			rdtsc
			
			mov esp, ebp
			pop ebp
			
			ret		
				
_debug:
        push    bp
        mov     bp, sp
        push	ax

vuelve:	mov     ax, 1
        cmp	ax, 0
		jne	vuelve
		pop	ax
		pop     bp
        retn

_out:
		push	ebp
		mov		ebp, esp		; Stack frame
		mov		edx, [ebp+8]   	; Puerto
		mov		eax, [ebp+12]  	; Lo que se va a mandar
		out		dx, al
		mov		esp, ebp
		pop		ebp
		ret

_inw:
		push	ebp
		mov		ebp, esp		; Stack frame
		mov		edx, [ebp+8]    ; Puerto
		mov		eax, 0          ; Limpio eax
		in		ax, dx
		mov		esp, ebp
		pop		ebp
		ret

_outw:
		push	ebp
		mov		ebp, esp		; Stack frame
		mov		edx, [ebp+8]   	; Puerto
		mov		eax, [ebp+12]  	; Lo que se va a mandar
		out		dx, ax
		mov		esp, ebp
		pop		ebp
		ret

_in:
		push	ebp
		mov		ebp, esp		; Stack frame
		mov		edx, [ebp+8]    ; Puerto
		mov		eax, 0          ; Limpio eax
		in		al, dx
		mov		esp, ebp
		pop		ebp
		ret

_inb:
		push	ebp
		mov		ebp, esp		; Stack frame
		mov		edx, [ebp+8]    ; Puerto
		mov		eax, 0          ; Limpio eax
		in byte		al, dx
		mov		esp, ebp
		pop		ebp
		ret

_outb:
		push	ebp
		mov		ebp, esp		; Stack frame
		mov		edx, [ebp+8]   	; Puerto
		mov		eax, [ebp+12]  	; Lo que se va a mandar
		out	dx, al
		mov		esp, ebp
		pop		ebp
		ret

_activatePaging:
		cli
		push	ebp
		mov		ebp, esp
		mov		eax, cr0
		or 		eax, 0x80000000
		mov 	cr0, eax
		mov		esp, ebp
		pop		ebp
		sti
		ret

_setPageDir:
		cli
		push	ebp
		mov		ebp, esp
		mov		edx, [ebp+8]
		mov		cr3, edx
		mov		esp, ebp
		pop		ebp
		sti
		ret

_printASM:
		mov		ebx, 0xb8000
		mov 	eax, 0
again:
		mov 	[ebx], eax
		add		eax, 1
		add		ebx, 2
		jmp		again

_readCR2:
		push	ebp
		mov		ebp, esp
		mov eax, cr2
		mov		esp, ebp
		pop		ebp
		ret

_getESP:
		push	ebp
		mov		ebp, esp
		mov		eax, esp
		mov		esp, ebp
		pop		ebp
		ret

