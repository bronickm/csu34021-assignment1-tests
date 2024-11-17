.data
buffer db "HeLlO", 0

.code
recursiveLowerCase PROC
    sub     rsp, 40                 ; local var. (8 bytes) + shadow space (32 bytes)

    xor     rax, rax                ; setting temp_aux to 0

    mov     r8b, [rcx]              ; retrieving current character
    cmp     r8b, 64                 ; comparing current character with 64
    jle     skip_character          ; if current character <= 64 jump to skip_character
    cmp     r8b, 91                 ; comparing current character with 91
    jge     skip_character          ; if current character >= 91 jump to skip_character
    inc     rax                     ; incrementing temp_aux
    add     r8b, 32                 ; adding 32 to the current character value
    mov     [rcx], r8b              ; storing current character back in memory

skip_character:
    dec     edx                     ; decrementing len
    cmp     edx, 0                  ; comparing len with 0
    jle     epilogue                ; if len <= 0 jump to epilogue

    inc     rcx                     ; incrementing buffer pointer
    mov     [rsp + 32], rax         ; aux = temp_aux
    call    recursiveLowerCase      ; calling recursiveLowerCase
    add     rax, [rsp + 32]         ; setting return value to aux + call result

epilogue:
    add     rsp, 40                 ; deallocating fixed stack space
    ret     0                       ; returning
recursiveLowerCase ENDP


main PROC
    sub     rsp, 40                 ; alignment (8 bytes) + shadow space (32 bytes)

    mov     rcx, OFFSET buffer      ; passing buffer pointer as the 1st arg. (s)
    mov     edx, 5                  ; passing buffer length as the 2nd arg. (len)
    call    recursiveLowerCase      ; calling recursiveLowerCase

    xor     rax, rax                ; setting the return value to 0

    add     rsp, 40                 ; deallocating fixed stack space
    ret     0                       ; returning
main ENDP

END
