.intel_syntax noprefix

# .code16
.text
    mov ax, cs
    mov ds, ax
    mov es, ax
    call DispStr # call display string
    jmp . # loop

DispStr:
    mov dx, 0
    mov ax, OFFSET FLAT: BootMessage
    mov bp, ax # ES:BP = string address
    mov cx, 16 # CX = string length
    mov ax, 0x1301 # AH = 13, AL = 01h
    mov bx, 0x003 # page number = 0 (BH = 0), bg = blace# word = red (BL = OCh)
LOOP1:
    mov dl, 0
LOOP2:
    int 0x10
    add dl, cl
    cmp dl, 60 
    jbe LOOP2
    add dh, 1
    cmp dh, 10
    jbe LOOP1
    ret

BootMessage: .string "Hello, OS world!"
# times 510-($-$$) db 0 # pading to 512 byte
.org 510 
# end
.word 0xaa55 # end mark
