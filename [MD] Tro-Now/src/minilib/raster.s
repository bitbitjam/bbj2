        .align  2
        .globl  vscroll_a
        .comm   vscroll_a,2,2
        .globl  start_line
        .comm   start_line,1,1

        .text
        .align  2
        .globl  BgAsmHIntCB
        .type   BgAsmHIntCB, @function
BgAsmHIntCB:
        movem.l d0-d1,-(sp)
        moveq #0,d0
        move.b start_line,d0
        jeq bg_asm_end
        move.w #0x4000,0xC00004
        move.b d0,d1
        subq.b #1,d1
        move.b d1,start_line
        add.w vscroll_a,d0
        move.w d0,0xC00000
bg_asm_end:
        movem.l (sp)+,d0-d1
        rte
        .size   BgAsmHIntCB, .-BgAsmHIntCB
