section .text

string_length:
    xor rax, rax
	.loop:
		cmp byte[rdi+rax],0
		je .end	 			;переход, если ZN=0
		inc rax 
		jmp .loop
	.end:
    	ret

print_string:
	call string_length
	mov rsi, rdi
	mov rdi, 1
	mov rdx, rax 	;количество байт для записи
	mov rax, 1 		;номер системного вызова write
	syscall
    ret

string_copy:
    call string_length 
	mov rcx, rax		; string length to rcx
	inc rcx				; go while not 0
	xchg rsi, rdi		; change position
	mov byte[rdi+rax], 0	; null-terminated	
	rep movsb			; copy repeat
	ret

	
print_char:
	push rdi 		;кладём символ
	mov rsi, rsp 	;вывод с вершины стека
	mov rdx, 1 		;количество байт для записи
	mov rax, 1 		;номер системного вызова write
	mov rdi, 1 		;stdout
	syscall
	pop rdi 		;забираем символ
    ret

print_newline:
	mov rdi, 0xA
    call print_char
    ret


print_uint:
	push rbp
	mov rbp, rsp			;в rbp теперь указатель на вершину стека
	mov rax, rdi			;переместить число в rax
	mov rdi, 10				;чтобы делить на 10
	sub  rsp, 32			;выделить место на стеке
	dec  rbp				;положить туда 0
	mov  byte[rbp], 0
	.loop:
		dec  rbp			;подвинуть указатель
	  xor  rdx, rdx
	  div  rdi				;разделить на 10
	  add  rdx, '0'			;добавить код нуля, чтобы получить цифру
	  mov  byte[rbp], dl	;положить на стек
	  test rax, rax			;закончить цикл, если всё число рассмотрено
	  jnz  .loop
	mov rdi, rbp			;печать числа
	call print_string
	add rsp, 32			;вернуть стек на место, убрать выделенный буфер
	pop rbp
    ret


print_int:
	test rdi, rdi 	;установка флагов
	jns .unsigned 	;если беззнаковое, печатать через print_uint
	push rdi 		;сохранить число
	mov rdi, '-' 	;напечатать минус
    call print_char ;восстановить число
	pop rdi
	neg rdi 		;сделать число положительным
	.unsigned:
		jmp print_uint

read_char:
	push 0
	mov rsi, rsp 	;читать с вершины стека
	mov rdx, 1 		;1 символ
	xor rdi, rdi
	xor rax, rax
	syscall
	pop rax
    ret 

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
	call string_length
	mov rcx, rax 	;в rcx лежит длина строки для цикла
	mov rsi, rdi
	xor rax, rax 	;тут будет результат
	xor rdx, rdx 	;тут будет длина итогового числа
	.loop:
		xor rdi, rdi 			;очистка rbx
		mov dil, byte[rsi+rdx] 	;в rbx лежит символ, который будет сейчас проверяться
		cmp dil, '0' 			;вычитается код нуля, чтобы проверить, что символ больше 0
		jb .end
		cmp dil, '9'				;вычитается код 9, чтобы проверить, что символ больше 9
		ja .end
		sub dil, '0' 			;вычесть код 0, чтобы получить цифру
		imul rax, 10 			;добавление цифры в rax
		add rax, rdi
		inc rdx 				;увеличивается длина итогового числа
		dec rcx 				;уменьшение количества итераций
		jnz .loop 				;продолжение, если 0	
	.end: 
		ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
  cmp byte[rdi], '-' 				
  je .signed
  jmp parse_uint
  .signed:
    inc rdi
    call parse_uint
    test rdx, rdx 					
    jz .null
    neg rax 								
    inc rdx
    ret
  .null:
    xor eax, eax 					
    ret


string_equals:
  call string_length					; rax - length
  mov rcx, rax	 							; rcx - length of the first
  xchg rdi, rsi 							; exchange rdi - second, rsi - first
  call string_length					; rax - length of the second
  cmp rax, rcx 								; check their lengths
  jne  .not_equals
  repe cmpsb 									; repe - repeat until not equals ; cmpsb - set flags
  jne  .not_equals
  mov rax, 1 									; return 1
  ret
  .not_equals:
    mov rax, 0 								; return 0
    ret
	
;в rdi указатель на стек
;в rsi размер стека
;в rdx будет размер строки
;в rax строка или 0 при переполнении
read_word:
	push r14
	push r15
	xor r14, r14	;тут счётчик
	mov r15, rsi   	;тут будет размер буфера
	dec r15
	.loop:				;читаем, пока там пробел/TAB
		push rdi
		call read_char
		pop rdi
		cmp al, ' '		
		je .loop
		cmp al, 13		
		je .loop
		cmp al, 10		
		je .loop
		cmp al, 9		;код TAB
		je .loop
		cmp al, 32		;код пробела
		je .loop	
		test al, al
		jz .end		
	.loop2:	
		mov byte[rdi + r14], al		;переносим первый символ
		inc r14						;счётчик
		push rdi
		call read_char				;следующий символ
		pop rdi
		cmp al, 0					;проверка, что следующий символ не пробел/TAB/перенос
		je .end
		cmp al, 13
		je .end
		cmp al, 10
		je .end
		cmp al, ' '
		je .end
		cmp al, 9
		je .end
		test al, al
		jz .end
		cmp r14, r15					;проверка на переполнение стека
		je .error
		jmp .loop2
	.end:
		mov byte[rdi + r14], 0 ;добавить 0 в конец строки
		mov rax, rdi 			;перенести в rax указатель на буфер
		mov rdx, r14			;длина строки в rdx
		pop r15
		pop r14
		ret
	.error:
		xor rax, rax		;при ошибке в rax 0
		xor rdx, rdx
		pop r15
		pop r14
		ret
		
print_error:
    xor rax, rax
	push rdi
	call string_length
	pop rsi
	mov rdx, rax 	;количество байт для записи
	mov rax, 1 		;номер системного вызова write
	mov rdi, 2		;stderr
	syscall
    ret


