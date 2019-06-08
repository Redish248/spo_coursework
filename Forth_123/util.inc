section .data
  xt_run: dq run
  xt_loop: dq start_loop
  program_stub: dq 0
  error_message: db "No such word!", 0
  mode: dq 0      
  was_branch: db 0
  here: dq forth_mem
  stack_start: dq 0


section .bss
  resq 1023
  rstack_start: resq 1
  forth_mem: resq 65536
  input_buf: resb 1024
  user_buf: resb 1024
  user_mem: resq 65536  ; current global
  state: resq 1

section .text
  find_word:
  xor eax, eax
  mov rsi, [last_word]

  .loop:
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi

    test rax, rax
    jnz .found
    mov rsi, [rsi]
    test rsi, rsi
    jnz .loop

    xor eax, eax
    ret

  .found:
    mov rax, rsi
    ret

cfaddr:
  xor eax, eax
  add rdi, 8
  push rdi
  call string_length
  pop rdi
  add rax, 1
  add rax, 1
  add rdi, rax
  mov rax, rdi
  ret

next:
  mov w, [pc]
  add pc, 8
  jmp [w]
