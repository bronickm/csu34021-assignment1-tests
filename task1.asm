.686
.MODEL flat, c                      ; specifying the cdecl calling convention

EXTERN printf : PROC                ; declaration of an external printf procedure

.data
format_str db "%d", 0               ; format string for the printf call

.code
my_product3 PROC
    push    ebp                     ; preserving ebp - setting up the stack frame
    mov     ebp, esp                ; (ebp use can be avoided but aids readability)
    sub     esp, 4                  ; space for local variable x

    mov     eax, [ebp + 8]          ; retrieving parameter a
    cmp     eax, 1                  ; comparing a with 1
    jle     epilogue                ; if a <= 1 return a
    mov     edx, eax                ; temp_x = a
    dec     eax                     ; decrementing a

    mov     ecx, [ebp + 16]         ; retrieving parameter c
    cmp     ecx, 1                  ; comparing c with 1
    jl      skip_c_multiplication   ; if c < 1 jump to skip_c_multiplication
    imul    edx, ecx                ; temp_x *= c
    dec     ecx                     ; decrementing c

skip_c_multiplication:
    push    ecx                     ; passing c as the 3rd arg. for the recursive call

    mov     ecx, [ebp + 12]         ; retrieving parameter b
    cmp     ecx, 1                  ; comparing b with 1
    jl      skip_b_multiplication   ; if b < 1 jump to skip_b_multiplication
    imul    edx, ecx                ; temp_x *= b
    dec     ecx                     ; decrementing b

skip_b_multiplication:
    push    ecx                     ; passing b as the 2nd arg. for the recursive call
    push    eax                     ; passing a as the 1st arg. for the recursive call
    mov     [ebp - 4], edx          ; x = temp_x

    call    my_product3             ; calling my_product3 recursively
    add     esp, 12                 ; cleaning up the stack

    mov     ecx, [ebp - 4]          ; retrieving x
    imul    eax, ecx                ; setting return value to x * my_product3 result

epilogue:
    mov     esp, ebp                ; cleaning up the stack       
    pop     ebp                     ; restoring ebp
    ret     0
my_product3 ENDP


my_product3_wrapper PROC
    push    ebp                     ; preserving ebp - setting up the stack frame
    mov     ebp, esp                ; (use of ebp could be avoided)

    mov     ecx, [ebp + 8]          ; retrieving parameter N
    sub     ecx, 2                  ; calculating N-2
    push    ecx                     ; passing N-2 as the third argument (c)
    inc     ecx                     ; calculating N-1
    push    ecx                     ; passing N-1 as the second argument (b)
    inc     ecx                     ; calculating N
    push    ecx                     ; passing N as the first argument (a)
    call    my_product3             ; calling my_product3
    add     esp, 12                 ; cleaning up the stack

    pop     ebp                     ; restoring ebp
    ret     0                       ; returning
my_product3_wrapper ENDP


main PROC
    mov     ecx, 4                  ; N = 4

    push    ecx                     ; passing N as the only argument
    call    my_product3_wrapper     ; calling my_product3_wrapper
    add     esp, 4                  ; cleaning up the stack

    push    eax                     ; passing the result as the 2nd arg.
    push    OFFSET format_str       ; passing the format string as the 1st arg.
    call    printf                  ; calling printf
    add     esp, 8                  ; cleaning up the stack

    xor     eax, eax                ; setting the return value to 0
    ret     0                       ; returning
main ENDP

END main
