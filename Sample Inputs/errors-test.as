.entry Next
.extern wNum,ber
STR: .asciz "aBcd",
MAIN: add $3, ,$4, $9
LOOP: ori $9, blabla, $2
.extern Next
   la val1
   jmp Next
Next: move $120, $4
LIST: .db 111111111111111111111111116,-9
   bgt $-4, 2, END
   la K
   sw $0, +4, $10
   bne $31-, $-9, LOOP
   call val1
   jmp + $4
   la 2$wNumber
.extern val1
   .dh + 2705611111111111111111
K: .dw --31,-12
END: stop*
.entry K
