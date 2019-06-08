native '+', plus
  pop rdx
  pop rax
  add rax, rdx
  push rax
  jmp next

native '-', minuse
  pop rdx
  pop rax
  sub rax, rdx
  push rax
  jmp next

native '*', mul
  pop rdx
  pop rax
  imul rdx
  push rax
  jmp next

native '/', div
  pop rcx
  pop rax
  cqo
  idiv rcx
  push rax
  jmp next

native '<', less
  pop rdx
  pop rax
  cmp rax, rdx
  setl al
  movzx rax, al
  push rax
  jmp next

native '=', equals
  pop rdx
  pop rax
  cmp rax, rdx
  jne .different
  xor rax, rax
  mov rax, 1
  push rax
  jmp next
  .different:
    xor rax, rax
    push rax
    jmp next

native "%", mod
  pop rcx
  pop rax
  cqo
  idiv rcx
  push rdx
  jmp next

native 'and', and
  pop rdx
  pop rax
  and rax, rdx
  push rax
  jmp next

native 'or', or
  pop rax
  or [rsp], rax
  jmp next

native 'not', not
  pop rax
  test rax, rax
  jz .one
  xor rax, rax
  push rax
  jmp next
  .one:
    xor rax, rax
    mov rax, 1
    push rax
    jmp next

native '!', val_by_addr
  pop rax
  pop rdx
  mov [rax], rdx
  jmp next

native '@', val_to_addr
  pop rax
  push qword[rax]
  jmp next

native 'c!', put_byte
  pop rax
  pop rdx
  mov [rax], dl
  jmp next

native 'c@', get_byte
  pop rax
  movzx rax, byte [rax]
  push rax
  jmp next

native '>r', to_rstack
  pop rax
  sub rstack, 8
  mov qword [rstack], rax
  jmp next

native 'r>', from_rstack
  mov rax, qword [rstack]
  add rstack, 8
  push rax
  jmp next

native "r@", get_rstack
  push qword [rstack]
  jmp next

native 'lor', lor
  pop rax
  pop rdx
  test rax, rax
  jnz .yes
  push rdx
  jmp next
  .yes:
    push rax
    jmp next

native "land", land
  pop rax
  pop rdx
  test rax, rax
  jz .no
  push rdx
  jmp next
  .no:
    push rax
    jmp next

native 'buf', buf
	push qword user_buf
	jmp next

native 'read_word', read_word
  pop rdi
  call read_word
  push rdx
  jmp next

native 'drop', drop
  add rsp, 8
  jmp next

native 'print_str', print_str
  pop rdi
  call print_string
  jmp next

native 'print_newline', print_newline
  call print_newline
  jmp next

native '.', dot
  pop rdi
  call print_int
  jmp next

native 'compare', compare
  pop rdi
  pop rsi
  call string_equals
  push rax
  jmp next

native 'dup', dup
  push qword [rsp]
  jmp next

native 'swap', swap
  pop rdx
  pop rax
  push rdx
  push rax
  jmp next

native '.S', show_stack
  mov rcx, rsp
  .loop:
    cmp rcx, [stack_start]
    jae next
    mov rdi, [rcx]
    push rcx
    call print_int
    call print_newline
    pop rcx
    add rcx, 8
    jmp .loop

native 'rot', rot
  pop rax
  pop rdx
  pop rcx
  push rdx
  push rax
  push rcx
  jmp next

native 'read_char', read_char
  call read_char
  push rax
  jmp next

native 'print_char', print_char
  pop rdi
  call print_char
  jmp next

native 'find', find
  pop rdi          
  call find_word
  push rax
  jmp next

native 'cfa', cfa
  pop rdi
	call cfaddr
  push rax
  jmp next

native 'init_cmd', init_cmd
  pop rax
  mov w, rax
  jmp [w]

native 'bye', bye
  mov rax, 60
  xor rdi, rdi
  syscall

native 'send_warning', send_warning
  mov  rdi, error_message
  call print_string
  call print_newline
  jmp next

native 'isimmediate', isimmediate
  pop rax
  push rax
  xor rdx, rdx
  mov dl, byte[rax - 1]
  push rdx
  jmp next

native 'parse_number', parse_number
  pop rdi
  call parse_int      
  push rax            
  push rdx
  jmp next

native 'pushmode', pushmode
  xor eax, eax
  mov rax, [mode]
  push rax
  jmp next

native 'pushmode_addr', pushmode_addr
  xor eax, eax
  mov rax, mode
  push rax
  jmp next

native ',', comma
  mov rax, [here]
  pop qword [rax]
  add qword [here], 8
  jmp next

native 'isbranch', isbranch
  pop rax
  push rax
  cmp byte[rax - 1], 2
  jne .continue
  mov byte[was_branch], 1
  .continue:
  jmp next

native 'unset_branch', unset_branch
  mov byte[was_branch], 0
  jmp next

native 'wasbranch', wasbranch
  xor rdx, rdx
  mov dl, byte[was_branch]
  push rdx
  jmp next

native 'pushlit', pushlit
  push xt_lit
  jmp next

native 'save_last', save_last
  push last_word
  jmp next

native 'lit', lit
  push qword [pc]
  add pc, 8
  jmp next

native 'save_word', save_word
  pop rax
  mov  qword[here], rax
  xor eax, eax
  mov rax, here
  add rax, 8
  add qword[here], rax
  jmp next

native 'save_number', save_number
  pop rax
  mov [here], rax
  xor eax, eax
  mov rax, here
  add rax, 8
  mov qword[here], rax
  jmp next

native 'init', init
  mov rstack, rstack_start
  mov stack, stack_start

  xor eax, eax
  mov rax, [in_fd]
  sub rstack, 8
  mov qword [rstack], rax
  xor eax, eax
  pop rax
  mov [in_fd], rax

  mov qword[mode], 0
  mov pc, xt_run
  cmp qword [stack_start], 0
  je  .first
  mov rsp, [stack_start]
  jmp next
  .first:
  mov [stack_start], rsp
  jmp next

native 'buffer', buffer
  push qword input_buf
  jmp next

native 'docol', docol
  sub rstack, 8
  mov qword [rstack], pc
  add w, 8
  mov pc, w
  jmp next

native 'exit', exit
  mov pc, [rstack]
  add rstack, 8
  jmp next

native 'branchifz', branchifz
  pop rax
  test rax, rax
  jnz .skip
  mov pc, [pc]
  jmp next
  .skip:
    add pc, 8
    jmp next

native 'branch', branch
  mov pc, [pc]
  jmp next

native 'create', create
  mov rcx, [last_word]
  mov rsi, [here]
  mov [rsi], rcx
  mov [last_word], rsi
  add rsi, 8
;??? skip ?
  ; name
  pop rdi
  push rsi
  call string_copy
  pop rsi
  push rsi
  mov rdi, rsi
  call string_length
  pop rsi
  add rsi, rax
  ; flags
  inc rsi      
  pop rax
  mov [rsi], al
  inc rsi

  mov [here], rsi

  jmp next

const dp, dp
const here, [here]
const in_fd, in_fd
const state, state
const mem, user_mem

native "syscall", syscall
  pop r9
  pop r8
  pop r10
  pop rdx
  pop rsi
  pop rdi
  pop rax
  syscall
  push rax
  push rdx
  jmp next


