: IMMEDIATE save_last @ cfa 1 - dup c@ 1 or swap c! ;

: if ' branchifz , here 0  , ; IMMEDIATE
: else ' branch , here 0 , swap here swap !  ; IMMEDIATE
: then here swap ! ; IMMEDIATE
: endif ' then init_cmd ; IMMEDIATE

: repeat here ; IMMEDIATE
: until ' branchifz , , ; IMMEDIATE

: over >r dup r> swap ;
: 2dup over over ;

: for
      ' swap ,
      ' >r ,
      ' >r ,
here  ' r> ,
      ' r> ,
      ' 2dup ,
      ' >r ,
      ' >r ,
      ' < ,
      ' branchifz ,
here    0 ,
       swap ; IMMEDIATE

: endfor
    ' r> ,
    ' lit , 1 ,
    ' + ,
    ' >r ,
   ' branch ,
   ,  here swap !
       ' r> ,
     ' drop ,
       ' r> ,
     ' drop ,

;  IMMEDIATE
