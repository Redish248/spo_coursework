colon ">", greater 
    dq xt_swap
    dq xt_less
    dq xt_exit

colon ":", colon
	.input:
		dq xt_buffer
		dq xt_read_word
		branchif0 .input
	dq xt_lit, 0
	dq xt_buffer
	dq xt_create
	dq xt_lit, 1, xt_pushmode_addr
	dq xt_val_by_addr
	dq xt_lit, docol_impl
	dq xt_comma
	dq xt_exit

colon "double", double
	dq xt_dup
	dq xt_plus
	dq xt_exit

colon "'", quotation, 1
	dq xt_buffer
	dq xt_read_word
	dq xt_drop
	dq xt_buffer
	dq xt_find, xt_dup
	branchif0 .error_word
	dq xt_cfa
	dq xt_pushmode
	branchif0 .interpret
	dq xt_lit, xt_lit
	dq xt_comma
	dq xt_comma
	.interpret:
		dq xt_exit
	.error_word:
		dq xt_send_warning
		dq xt_exit


colon ";", semicolon, 1
    dq xt_lit, 0, xt_pushmode_addr
	dq xt_val_by_addr
    dq xt_lit, xt_exit
	dq xt_comma
    dq xt_exit




