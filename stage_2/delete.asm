section .data

int_inMsg    db        "Enter an integer value" , 10, 0
real_inMsg   db        "Enter a real value", 10, 0
bool_inMsg   db        "Enter a boolean value", 10, 0
arr_inMsg    db        "Enter %d elements of %s type for array range %d to %d", 10, 0
intFormat     db        "%d", 0
type_int      db        "integer", 0
type_real     db        "real", 0
type_bool     db        "boolean", 0
intFormat_in  db        "%d", 0
intFormat_out db        "%d ", 0
realFormat_in db        "%lf", 10, 0
realFormat_out db       "%lf", 0
strFormat_in   db       "%s", 0
strFormat_out   db       "%s", 10, 0
true_label     db        "true ", 0
false_label     db        "false ", 0
arr_outMsg   db        "Printing array: ", 0
new_line       db       10, 0
var1         dd        3
var2         dd        6
array_available_addr   dd   0



		section .bss

vard1:   resd    1

int_array:         resd        100
real_array:        resq        100
bool_array:        resb        100
array_buffer:      resq        1000


global main
extern printf
extern scanf
extern puts
extern exit

section  .text

main:

push rbp
mov rbp, rsp
	sub rsp, 16
	sub rsp, 16
	mov r14, [array_available_addr]
	lea rax, [array_buffer + r14 * 8]
	mov [rbp - 64], rax

	mov r14, [array_available_addr]
	add r14, 15
	mov [array_available_addr], r14

; loading array offsets
mov r13, 1
mov [rbp - 72], r13
mov r13, 15
mov [rbp - 80], r13

	sub rsp, 16
	mov r14, [array_available_addr]
	lea rax, [array_buffer + r14 * 8]
	mov [rbp - 88], rax

	mov r14, [array_available_addr]
	add r14, 15
	mov [array_available_addr], r14

; loading array offsets
mov r13, 1
mov [rbp - 96], r13
mov r13, 15
mov [rbp - 104], r13

	sub rsp, 16
	;Taking id

	mov rdi, int_inMsg
	call printf
	mov rdi, intFormat_in
	lea rsi, [rbp - 112]
	call scanf
	;Printing ID

	mov rdi, realFormat_out
	mov rsi, [rbp - 120]
	call printf

	lea rdi, [new_line]
	call printf

mov rsp, rbp
pop rbp
mov rax, 0
ret
