.syntax unified
.cpu cortex-m0
.text
    .thumb
    
    .thumb_func
    .global OE_delay
OE_delay:
    lsls r1, r0
    beq.n exit_func
    movs r3, #0
loop:
    nop
    adds r3, #32
    cmp r1, r3
    bhi.n loop
exit_func:
    bx lr 
