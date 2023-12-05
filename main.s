	.intel_syntax noprefix

	.text
	.section	.rodata
.HELLO:
	.string	"Testing testing"
.SEPARATOR:
	.string	"==================="
.XPRINT:
	.string	"xOffs: %d\n"

# 	.text
# 	.type	clrNewlines, @function
# /** Set first newline in string to \0 */
# clrNewlines:
# 	push	rbp
# 	mov	rbp, rsp
# 
# 	cmp rdi, 0
# 	je exitLoop
# 
# loop:
# 	mov rax, [rdi]
# 	cmp rax, '\n
# 	jne noMatch
# 	movb [rdi], 0
# 	jmp exitLoop
# noMatch:
# 	inc rdi
# 	cmp rax, 0
# 	jne loop
# 
# exitLoop:
# 
# 	pop rbp
# 	ret
# 	.size	clrNewlines, .-clrNewlines
# 
# 	.text
# 	.type	sumSurroundings, @function
# /** Söker efter tal runtom den givna symbolen och summerar dom till totalen. Tar buffern som första parametern och symbol indexet som andra. */
# sumSurroundings:
# 	push	rbp
# 	mov	rbp, rsp
# 	push	r15
# 	mov r15, rdi
# 
# 	# När vi hittar en symbol, sök positionerna på raden över -1,0,+1, mittersta raden -1,+1 och undre raden -1,0,+1
# 
# 	# Loopa över alla rutor runtom symbolen
# 	mov r10, -1 # X-offs
# 	# mov r11, -1 # Y-offs, börjar med bara X
# 
# sumSurroundingsLoop:
# 	mov r9, r15
# 	add r9, 1000
# 	add r9, rsi
# 	add r9, r10 # r9 = buffer[1][symbolOffs + xOffs]
# 	mov r9, [r9] # Dereference
# 
# 	lea rdi, [.XPRINT]
# 	mov rsi, r9
# 	call printf
# 
# 	# Kolla om nuvarande position matchar en siffra
# 	cmp r9, '0
# 	jl noNumMatch
# 	cmp r9, '9
# 	jg noNumMatch
# 
# 	# Match!
# 
# noNumMatch:
# 	inc r10 # Increment xOffs, skip xOffs=0
# 	jnz xOffsNotZero
# 	inc r10
# xOffsNotZero:
# 
# 	cmp r10, 2 # Exit loop when xOffs == 2
# 	je sumSurroundingsExit
# 
# 	jmp sumSurroundingsLoop
# 
# 	# Om nuvarande positionen innehåller en siffra så har vi hittat en match. Sök bakåt för att hitta första indexet, sök sen framåt för att hitta slutindexet.
# 
# 	# Parsa siffrorna till ett decimaltal, summera talet till en global result variabel.
# 	# För att undvika double counting, returnera hur många tecken höger talet extendar så omgivningssöken kan skippa dom.
# 
# sumSurroundingsExit:
# 	pop r15
# 	pop rbp
# 	ret
# 	.size	sumSurroundings, .-sumSurroundings

	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp

# 	lea edi, [.HELLO]
# 	call	puts
# 
# 	# Allokera en buffer som innehåller 3 rader text
# 	# Fyll buffern med 0:or
# 	mov rdi, 3 # 3 rader, tusen långa
# 	mov rsi, 1000
# 	call calloc
# 	mov r15, rax # Spara rad-buffern i r15
# 
# lineLoop:
# 	# Successivt skifta upp alla rader 1 steg och läs en rad från stdin in i nedersta raden
# 	mov rdi, r15 # dest: buffer[0]
# 	mov rsi, r15 # src: buffer[1]
# 	add rsi, 1000
# 	mov rdx, 1000 # len: 1000
# 	call memcpy
# 
# 	mov rdi, r15 # dest: buffer[1]
# 	add rdi, 1000
# 	mov rsi, r15 # src: buffer[2]
# 	add rsi, 2000
# 	mov rdx, 1000 # len: 1000
# 	call memcpy
# 
# 	# Clear buffer[2] before reading new line, to prevent old data lingering
# 	mov rdi, r15
# 	add rdi, 2000
# 	mov rsi, 0
# 	mov rdx, 1000
# 	call memset
# 
# 	# Read newline into buffer[2]
# 	mov rdi, r15 # str: buffer[2]
# 	add rdi, 2000
# 	mov rsi, 1000 # n: 1000
# 	mov rdx, stdin # stream: STDIN
# 	call fgets
# 	mov r14, rax
# 
# 	mov rdi, r14 # Clear newlines in the fetched line
# 	call clrNewlines
# 
# 	# Print current buffer
# 	mov rdi, r15
# 	call puts
# 	mov rdi, r15
# 	add rdi, 1000
# 	call puts
# 	mov rdi, r15
# 	add rdi, 2000
# 	call puts
# 
# 	# Vi måste söka genom mittersta raden efter symboler
# 	# En symbol verkar kunna vara nåt av vanliga #$* symbolerna men för att vara säker, anse allt annat än siffror, punkter och \0 att vara en symbol
# 	# (använd stdlib eller skriv nåt själv)
# 	mov r13, r15 # Current line pointer = buffer[1]
# 	add r13, 1000
# .symbolSearchLoop:
# 	# Är den här bokstaven en symbol?
# 	movb al, [r13]
# 	cmp al, '0
# 	jge notASymbol
# 	cmp al, '9
# 	jle notASymbol
# 	cmp al, '.
# 	je notASymbol
# 	cmp al, 0
# 	je exitSymbolLoop
# 
# 	# Är en symbol! Summera upp alla närliggande tal
# 	mov rdi, r15 # buffer
# 	mov rsi, r13 # symbol index on line, r13 - (r15 + 1000)
# 	sub rsi, r15
# 	sub rsi, 1000
# 	call sumSurroundings
# 
.testlabel:
	nop
	jmp testlabel
# 	inc r13
# 	jmp symbolSearchLoop
# .exitSymbolLoop:
# 
# 	lea edi, [.SEPARATOR]
# 	call	puts
# 
# 	# Loopa så över varje rad, lägg till en fake rad i slutet så sista raden hamnar i mittenbuffern
# 	# (Kanske inte behöver göra det beroende på hur EOF hanteras)
# 	cmp r14, 0
# 	jne lineLoop


	# Printa resultatvärdet

	mov	eax, 0

	pop	rbp
	ret
	.size	main, .-main

	.section	.note.GNU-stack,"",@progbits
