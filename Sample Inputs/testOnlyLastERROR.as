;file ps.as
;sample source code

.entry	Next a
.extern   wNumber b
STR:	.asciz	"aBcd"a
MAIN: add	$3,$5,$9b 
LOOP: ori	$9,-5,$2c
	la	val1 d
	jmp	Next e
Next:	move	$20,$4f 
LIST:   .db	6,-9g
	bgt	$4,$2,END h
	la	K i 
	sw	$0,4,$10j
	bne	$31,$9, LOOP k
	call	val1 l
	jmp	$4m
	la	wNumber n
.extern val1 o
	.dh	27056p
K:	.dw	31,-12q
END:	stop r
.entry	K s
