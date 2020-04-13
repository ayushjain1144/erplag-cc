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
var1         dd        7
var2         dd        14



section .bss

vard1:   resd    1

int_array:         resd        100
real_array:        resq        100
bool_array:        resb        100


global main
extern printf
extern scanf
extern puts
extern exit

section  .text

main:

push rbp
; prompts user for input
mov rcx, [var1]
mov r8, [var2]
mov rsi, r8
sub rsi, rcx
mov rdi, arr_inMsg
mov rdx, type_bool
mov rcx, [var1]
mov r8, [var2]
call printf

; stores the count

xor r12, r12
xor r13, r13
mov r12d, [var2]
mov r13d, [var1]
sub r12d, r13d
;mov r12, 5
xor r13, r13

array_input_loop:
lea rdi, [intFormat_in]
lea rsi, [bool_array + r13d * 1]
call scanf

inc r13d
cmp r13d, r12d
jne array_input_loop
pop rbp


push rbp
; printing array
lea rdi, [arr_outMsg]
call printf
; stores the count

xor r12, r12
xor r13, r13
mov r12d, [var2]
mov r13d, [var1]
sub r12d, r13d
;mov r12, 5
xor r13, r13

array_output_loop:
lea rdi, [strFormat_in]
mov sil, [bool_array + r13d * 1]

cmp sil, 0
jz false
lea rsi, [true_label]
jmp print

false: 
lea rsi, [false_label]

print: 
call printf

inc r13d
cmp r13d, r12d
jne array_output_loop
lea rdi, [new_line]
call printf
pop rbp


mov rax, 0
ret
