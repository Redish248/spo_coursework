: isparity 2 % 0 = if 1 else 0 then ;


: isprime 
dup 1 < if ." Incorrect argument " else
	dup 
	3 < if drop 1 else ( numbers 1 and 2 are prime )
		0 swap
		dup 1 + ( count loops)
		1 ( first index)
		for
			dup
			r@ % ( count remainder of the division )
			0 = if swap 1 + swap then ( add counter if divided completely)
		endfor 
		swap
		3 < if drop 1 else drop 0 then ( 1 -- prime, 0 -- compound)
	then
then
; 


: prime-allot
isprime 
1 allot
dup rot swap
!
;


: concat
over count over count + 1 + ( count length of two string )
heap-alloc dup >r ( send allocated address to stack )
dup rot
dup count >r ( get length of first string )
dup >r ( and send it to stack )
string-copy ( copy first string )
r>
heap-free ( delete first string )
r> + ( increase address of allocated memory )
swap dup >r
string-copy ( copy second string )
r>
heap-free ( delete second string )
r>
prints ( print result )
;


: radical
dup 1 < if ." Incorrect argument " else
	1 1 >r ( set first index and send result to stack )
	repeat
		1 + ( increase index )
		over over % ( check division )
		0 = if
			dup r> * >r ( increase result )
			repeat
			dup rot swap / swap
			over over % 0 >
			until ( repeat division until number % index != 0)
		then
	over 1 =
	until ( repeat until number nor equals to 1 )
	drop drop
	r>
then
;


