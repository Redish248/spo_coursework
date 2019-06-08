%include 'lib.inc'
%include 'macro.inc'
%include 'util.inc'
%include 'words.inc'
%include 'colon_words.inc'

global _start

section .data
  last_word: dq link
  dp: dq user_mem
  in_fd: dq 0

section .text
_start:
  xor eax, eax
  push rax         
  jmp init_impl

run:
  dq docol_impl

  start_loop:
    dq xt_buffer
    dq xt_read_word           
    branchif0 exit          
    dq xt_buffer
    dq xt_find 
    dq xt_pushmode
    branchif0 .interpreter

  .compiler:

    dq xt_dup                
    branchif0 .comp_numb
    dq xt_cfa     
    dq xt_isimmediate
    branchif0 .notImmediate

    .immediate:
      dq xt_init_cmd
      branch start_loop

    .notImmediate:
      dq xt_isbranch
      dq xt_comma
      branch start_loop

    .comp_numb:
      dq xt_drop
      dq xt_buffer
      dq xt_parse_number

      branchif0 .error
      dq xt_wasbranch
      branchif0 .lit

      dq xt_unset_branch
      dq xt_save_number
      branch start_loop

  	.lit:
  		dq xt_lit, xt_lit
      dq xt_comma
      dq xt_comma
      branch start_loop

  .interpreter:
    dq xt_dup
    branchif0 .interpreter_numb

    dq xt_cfa                 
    dq xt_init_cmd
    branch start_loop

    .interpreter_numb:
      dq xt_drop
      dq xt_buffer
      dq xt_parse_number
      branchif0 .error
      branch start_loop

  .error:
    dq xt_drop
  	dq xt_send_warning
    branch start_loop

exit:
  dq xt_bye
