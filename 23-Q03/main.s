	.intel_syntax noprefix

	.text
	.section	.rodata
.HELLO:
	.string	"Testing testing"
.SEPARATOR:
	.string	"==================="
.XPRINT:
	.string	"xOffs: %d, %c\n"
.FOUND:
	.string	"Found match!"
.NUMBER:
	.string	"Number: %d\n"
.ANSWER:
	.string	"Answer: %d\n"

	.data
sum:
	.short 0

	.text
	.type	clrNewlines, @function
/** Set first newline in string to \0 */
clrNewlines:
	push	rbp
	mov	rbp, rsp

	cmp rdi, 0
	je exitLoop

loop:
	mov rax, [rdi]
	cmp rax, '\n
	jne noMatch
	movb [rdi], 0
	jmp exitLoop
noMatch:
	inc rdi
	cmp rax, 0
	jne loop

exitLoop:

	pop rbp
	ret
	.size	clrNewlines, .-clrNewlines

	.text
	.type	sumSurroundings, @function
/** Söker efter tal runtom den givna symbolen och summerar dom till totalen. Tar buffern som första parametern och symbol indexet som andra. */
sumSurroundings:
	push	rbp
	mov	rbp, rsp
	push	r15
	push	r14
	push	r13
	push	r12
	mov r15, rdi
	mov r14, rsi

	# När vi hittar en symbol, sök positionerna på raden över -1,0,+1, mittersta raden -1,+1 och undre raden -1,0,+1

	# Loopa över alla rutor runtom symbolen
	mov r12, -1 # X-offs
	mov r13, -1 # Y-offs

sumSurroundingsLoop:
	mov r9, r15 # r9 = buffer[1 + yOffs][symbolOffs + xOffs]
	add r9, 1000
	mov rax, 1000
	mul r13
	add r9, rax
	add r9, r14
	add r9, r12
	movb r9b, [r9] # Dereference
	and r9, 0xFF

	lea rdi, [.XPRINT]
	mov rsi, r12
	mov rdx, r9
	mov eax, 0
	push r9
	call printf
	pop r9

	# Kolla om nuvarande position matchar en siffra
	cmp r9, '0
	jl noNumMatch
	cmp r9, '9
	jg noNumMatch

	# Match!
	lea rdi, [.FOUND]
	call puts

	call parseNumber # Parse the number from this match, increment xOffs to the end of the number

noNumMatch:
	inc r12 # Increment xOffs, skip xOffs=0 yOffs=0
	cmp r12, 0
	jne isNotOrigo
	cmp r13, 0
	jne isNotOrigo
	inc r12
isNotOrigo:

	cmp r12, 2 # Increment y when xOffs == 2
	jl sumSurroundingsLoop
	mov r12, -1
	inc r13
	# Check if yOffs is 2, if so, exit loop
	cmp r13, 2
	jl yOffsTwo
	jmp sumSurroundingsExit
yOffsTwo:

	jmp sumSurroundingsLoop

sumSurroundingsExit:
	pop r12
	pop r13
	pop r14
	pop r15
	pop rbp
	ret
	.size	sumSurroundings, .-sumSurroundings

	.text
	.type	parseNumber, @function
parseNumber:
	push	rbp
	mov	rbp, rsp

	# Om nuvarande positionen innehåller en siffra så har vi hittat en match. Sök bakåt för att hitta första indexet, sök sen framåt för att hitta slutindexet.
	lea r9, [r15 + 1000] # digitPtr = buffer[1 + yOffs][symbolOffs + xOffs]
	mov rax, 1000
	mul r13
	add r9, rax
	add r9, r14
	add r9, r12

	# Sök bakåt
	mov r8, r9
stepBack:
	dec r8
	movb al, [r8]
	cmp al, '0
	jl noNumMatch2
	cmp al, '9
	jg noNumMatch2
	# Is number
	jmp stepBack
noNumMatch2:
	inc r8 # Hittade första icke-nummer byten. inkrementera tillbaks och parsa talet

	# Parsa siffrorna till ett decimaltal
	mov rax, 0 # Summera siffrorna in i rax
parseDigit:
	imul rax, 10
	xor rsi, rsi
	movb sil, [r8]
	sub rsi, '0
	add rax, rsi # rax = rax * 10 + (*r8 - '0')

	# Gå till nästa bokstav och se så det är en siffra
	inc r8
	movb dl, [r8]
	cmp dl, '0
	jl noNumMatch3
	cmp dl, '9
	jg noNumMatch3
	# Is number
	jmp parseDigit # Parse next digit
noNumMatch3:
	# Is not number, decrement our pointer back down to the last digit
	dec r8
	# Compute difference between last digit pointer and initial hit pointer
	mov rsi, r8
	sub rsi, r9
	# Add difference to xOffs to skip the rest of the digits in search
	add r12, rsi

	push rdi
	push rsi
	push rax
	lea rdi, [.NUMBER]
	mov rsi, rax
	mov eax, 0
	push r9
	call printf
	pop r9
	pop rax
	pop rsi
	pop rdi

	# Summera talet till en global result variabel.
	# För att undvika double counting, returnera hur många tecken höger talet extendar så omgivningssöken kan skippa dom.
	mov rsi, [sum]
	add rsi, rax
	mov [sum], rsi

	pop	rbp
	ret
	.size	parseNumber, .-parseNumber

	.text
	.globl	main
	.type	main, @function
main:
	push	rbp
	mov	rbp, rsp

	lea edi, [.HELLO]
	call	puts

	# Allokera en buffer som innehåller 3 rader text
	# Fyll buffern med 0:or
	mov rdi, 3 # 3 rader, tusen långa
	mov rsi, 1000
	call calloc
	mov r15, rax # Spara rad-buffern i r15

lineLoop:
	# Successivt skifta upp alla rader 1 steg och läs en rad från stdin in i nedersta raden
	mov rdi, r15 # dest: buffer[0]
	mov rsi, r15 # src: buffer[1]
	add rsi, 1000
	mov rdx, 1000 # len: 1000
	call memcpy

	mov rdi, r15 # dest: buffer[1]
	add rdi, 1000
	mov rsi, r15 # src: buffer[2]
	add rsi, 2000
	mov rdx, 1000 # len: 1000
	call memcpy

	# Clear buffer[2] before reading new line, to prevent old data lingering
	mov rdi, r15
	add rdi, 2000
	mov rsi, 0
	mov rdx, 1000
	call memset

	# Read newline into buffer[2]
	mov rdi, r15 # str: buffer[2]
	add rdi, 2000
	mov rsi, 1000 # n: 1000
	mov rdx, stdin # stream: STDIN
	call fgets
	mov r14, rax

	mov rdi, r14 # Clear newlines in the fetched line
	call clrNewlines

	# Print current buffer
	mov rdi, r15
	call puts
	mov rdi, r15
	add rdi, 1000
	call puts
	mov rdi, r15
	add rdi, 2000
	call puts

	# Vi måste söka genom mittersta raden efter symboler
	# En symbol verkar kunna vara nåt av vanliga #$* symbolerna men för att vara säker, anse allt annat än siffror, punkter och \0 att vara en symbol
	# (använd stdlib eller skriv nåt själv)
	mov r13, r15 # Current line pointer = buffer[1]
	add r13, 1000
symbolSearchLoop:
	# Är den här bokstaven en symbol?
	movb al, [r13]
	cmp al, '0
	jl notANumber
	cmp al, '9
	jg notANumber
	jmp notASymbol
notANumber:
	cmp al, '.
	je notASymbol
	cmp al, 0
	je exitSymbolLoop

	# Är en symbol! Summera upp alla närliggande tal
	mov rdi, r15 # buffer
	mov rsi, r13 # symbol index on line, r13 - (r15 + 1000)
	sub rsi, r15
	sub rsi, 1000
	call sumSurroundings

notASymbol:
	inc r13
	cmpb [r13], 0
	je exitSymbolLoop
	jmp symbolSearchLoop
exitSymbolLoop:

	lea edi, [.SEPARATOR]
	call	puts

	# Loopa så över varje rad, lägg till en fake rad i slutet så sista raden hamnar i mittenbuffern
	# (Kanske inte behöver göra det beroende på hur EOF hanteras)
	cmp r14, 0
	jne lineLoop

	# Printa resultatvärdet
	lea rdi, [.ANSWER]
	mov rsi, [sum]
	mov rax, 0
	call printf

	mov	eax, 0

	pop	rbp
	ret
	.size	main, .-main

	.section	.note.GNU-stack,"",@progbits

