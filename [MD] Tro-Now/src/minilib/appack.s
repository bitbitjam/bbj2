| -------------------------------------------------------------------------------------------------
| Aplib decruncher for MC68000 "gcc version"
| by MML 2010
| Size optimized (164 bytes) by Franck "hitchhikr" Charlet.
| -------------------------------------------------------------------------------------------------

| Make the function visible to the linker
.global aplib_decrunch

| -------------------------------------------------------------------------------------------------
| aplib_decrunch: A0 = Source / A1 = Destination
| void aplib_decrunch(unsigned char *source, unsigned char *destination); /* c prototype */
| -------------------------------------------------------------------------------------------------
aplib_decrunch:         movem.l 4(a7),a0-a1     | Recover params from the stack
                        movem.l a2-a5/d2-d6,-(a7)
                        lea     (24,a0),a0      | Skip 24 bytes from the header added for appack.exe
                        lea     32000.w,a3
                        lea     1280.w,a4
                        lea     128.w,a5
                        moveq   #1,d5           | Initialize bits counter
copy_byte:              move.b  (a0)+,(a1)+
next_sequence_init:     moveq   #0,d1           | Initialize LWM
next_sequence:          bsr.b   get_bit
                        bcc.b   copy_byte       | if bit sequence is %0..., then copy next byte
                        bsr.b   get_bit
                        bcc.b   code_pair       | if bit sequence is %10..., then is a code pair
                        moveq   #0,d0           | offset = 0 (eor.l d0,d0)
                        bsr.b   get_bit
                        bcc.b   short_match     | if bit sequence is %110..., then is a short match

                        | The sequence is %111..., the next 4 bits are the offset (0-15)
                        moveq   #4-1,d6
get_3_bits:             bsr.b   get_bit
                        roxl.l  #1,d0
                        dbf     d6,get_3_bits   | (dbcc doesn't modify flags)
                        beq.b   write_byte      | if offset == 0, then write 0x00

                        | If offset != 0, then write the byte on destination - offset
                        move.l  a1,a2
                        suba.l  d0,a2
                        move.b  (a2),d0
write_byte:             move.b  d0,(a1)+
                        bra.b   next_sequence_init
| Code pair %10...
code_pair:              bsr.b   decode_gamma
                        move.l  d2,d0           | get the new offset
                        subq.l  #2,d0           | offset == 2?
                        bne.b   normal_code_pair
                        tst.w   d1              | LMW == 0?
                        bne.b   normal_code_pair
                        move.l  d4,d0           | offset = old_offset
                        bsr.b   decode_gamma
                        bra.b   copy_code_pair
normal_code_pair:       add.l   d1,d0           | (d1 is either 1 or 0)
                        subq.l  #1,d0           | offset -= 1 (or 0)
                        lsl.l   #8,d0           | offset << 8
                        move.b  (a0)+,d0        | get the least significant byte of the offset (16 bits)
                        bsr.b   decode_gamma
                        cmp.l   a3,d0           | >=32000
                        blt.b   compare_1280
                        addq.l  #2,d2           | length += 2
                        bra.b   continue_short_match
compare_1280:           cmp.l   a4,d0           | >=1280 <32000
                        blt.b   compare_128
                        addq.l  #1,d2           | length++
                        bra.b   continue_short_match
compare_128:            cmp.l   a5,d0           | >=128 <1280
                        bge.b   continue_short_match
                        addq.l  #2,d2          | length += 2
                        bra.b   continue_short_match

| get_bit: Get bits from the crunched data (D3) and insert the most significant bit in the carry flag.
get_bit:                subq.b  #1,d5           | D5 = bit counter
                        bne.b   still_bits_left
                        moveq   #8,d5
                        move.b  (a0)+,d3        | Read next crunched byte
still_bits_left:        add.b   d3,d3           | D3.b << 1 (lsl.b #1,d3 o roxl.b #1,d3)
                        rts

| decode_gamma: Decode values from the crunched data using gamma code
decode_gamma:           moveq   #1,d2
get_more_gamma:         bsr.b   get_bit
                        addx.l  d2,d2
                        bsr.b   get_bit
                        bcs.b   get_more_gamma
                        rts

| Short match %110...
short_match:            moveq   #3,d2           | length = 3
                        move.b  (a0)+,d0        | Get offset (offset is 7 bits + 1 bit to mark if copy 2 or 3 bytes)
                        lsr.b   #1,d0
                        beq.b   end_decrunch    | if offset == 0, end of decrunching
                        bcs.b   continue_short_match
                        moveq   #2,d2           | length = 2
continue_short_match:   move.l  d0,d4           | old_offset = offset
copy_code_pair:         subq.l  #1,d2           | length--
                        move.l  a1,a2
                        suba.l  d0,a2
loop_do_copy:           move.b  (a2)+,(a1)+
                        dbf     d2,loop_do_copy
                        moveq   #1,d1           | LWM = 1
                        bra.w   next_sequence   | Process next sequence

end_decrunch:           movem.l (a7)+,a2-a5/d2-d6
                        rts
