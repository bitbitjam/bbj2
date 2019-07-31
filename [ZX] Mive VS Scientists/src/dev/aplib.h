extern unsigned int ram_address [];
extern unsigned int ram_destination [];
#ifdef MODE_128K
extern unsigned char ram_page [];
#endif
#asm

; aPPack decompressor
; original source by dwedit
; very slightly adapted by utopian
; optimized by Metalbrain

;hl = source
;de = dest

.depack		ld	ixl,128
.apbranch1	ldi
.aploop0	ld	ixh,1		;LWM = 0
.aploop		call 	ap_getbit
		jr 	nc,apbranch1
		call 	ap_getbit
		jr 	nc,apbranch2
		ld 	b,0
		call 	ap_getbit
		jr 	nc,apbranch3
		ld	c,16		;get an offset
.apget4bits	call 	ap_getbit
		rl 	c
		jr	nc,apget4bits
		jr 	nz,apbranch4
		ld 	a,b
.apwritebyte	ld 	(de),a		;write a 0
		inc 	de
		jr	aploop0
.apbranch4	and	a
		ex 	de,hl 		;write a previous byte (1-15 away from dest)
		sbc 	hl,bc
		ld 	a,(hl)
		add	hl,bc
		ex 	de,hl
		jr	apwritebyte
.apbranch3	ld 	c,(hl)		;use 7 bit offset, length = 2 or 3
		inc 	hl
		rr 	c
		ret 	z		;if a zero is encountered here, it is EOF
		ld	a,2
		adc	a,b
		push 	hl
		ld	iyh,b
		ld	iyl,c
		ld 	h,d
		ld 	l,e
		sbc 	hl,bc
		ld 	c,a
		jr	ap_finishup2
.apbranch2	call 	ap_getgamma	;use a gamma code * 256 for offset, another gamma code for length
		dec 	c
		ld	a,c
		sub	ixh
		jr 	z,ap_r0_gamma		;if gamma code is 2, use old r0 offset,
		dec 	a
		;do I even need this code?
		;bc=bc*256+(hl), lazy 16bit way
		ld 	b,a
		ld 	c,(hl)
		inc 	hl
		ld	iyh,b
		ld	iyl,c

		push 	bc
		
		call 	ap_getgamma

		ex 	(sp),hl		;bc = len, hl=offs
		push 	de
		ex 	de,hl

		ld	a,4
		cp	d
		jr 	nc,apskip2
		inc 	bc
		or	a
.apskip2	ld 	hl,127
		sbc 	hl,de
		jr 	c,apskip3
		inc 	bc
		inc 	bc
.apskip3		pop 	hl		;bc = len, de = offs, hl=junk
		push 	hl
		or 	a
.ap_finishup	sbc 	hl,de
		pop 	de		;hl=dest-offs, bc=len, de = dest
.ap_finishup2	ldir
		pop 	hl
		ld	ixh,b
		jr 	aploop

.ap_r0_gamma	call 	ap_getgamma		;and a new gamma code for length
		push 	hl
		push 	de
		ex	de,hl

		ld	d,iyh
		ld	e,iyl
		jr 	ap_finishup


.ap_getbit	ld	a,ixl
		add	a,a
		ld	ixl,a
		ret	nz
		ld	a,(hl)
		inc	hl
		rla
		ld	ixl,a
		ret

.ap_getgamma	ld 	bc,1
.ap_getgammaloop	call 	ap_getbit
		rl 	c
		rl 	b
		call 	ap_getbit
		jr 	c,ap_getgammaloop
		ret

#endasm

#asm
	._ram_address
		defw 0
	._ram_destination
		defw 0
#endasm


#ifdef COMPRESS_SCREENS
#asm
; -----------------------------------------------------------------------------
; RCS on-screen decoder by Einar Saukas & Antonio Villena (58 bytes)
; -----------------------------------------------------------------------------
; No parameters needed.
; -----------------------------------------------------------------------------

.drcs_onscreen
        ld      b, $02
.drcs_next
        ld      de, 256*(64)+1
.drcs_loop
        ld      h, d
        ld      a, e
        djnz    drcs_conv2 ; in first pass, skip first conversion

; In second pass, convert from format DA = x0x1x0R1R2c1c2c5 r1c3c4p1p2r2r3p3
;                           to format HA = x0x1x0R1R2p1p2r2 r3p3r1c3c4c1c2c5
        rrca               ; A = p3r1c3c4p1p2r2r3
        rrca               ; A = r3p3r1c3c4p1p2r2
        ld      c, a       ; C = r3p3r1c3c4p1p2r2
        xor     d
        and     $07
        xor     d          ; A = x0x1x0R1R2p1p2r2
        ld      h, a       ; H = x0x1x0R1R2p1p2r2
        xor     d
        xor     c          ; A = r3p3r1c3c4c1c2c5

; In first pass, convert from RCS format HA = x0x1x0R1R2c1c2c3 c4c5r1r2r3p1p2p3
;                              to format HL = x0x1x0R1R2c1c2c5 r1c3c4p1p2r2r3p3
; In second pass, convert from format HA = x0x1x0R1R2p1p2r2 r3p3r1c3c4c1c2c5
;            to regular screen format HL = x0x1x0R1R2p1p2p3 r1r2r3c1c2c3c4c5
.drcs_conv2
        ld      l, a      ; L = r3p3r1c3c4c1c2c5
        rlca              ; A = p3r1c3c4c1c2c5r3
        rrc     h         ; H = r2x0x1x0R1R2p1p2 (r2 to carry)
        rla               ; A = r1c3c4c1c2c5r3r2 (p3 to carry)
        rl      h         ; H = x0x1x0R1R2p1p2p3
        ld      c, a      ; C = r1c3c4c1c2c5r3r2
        xor     l
        and     $05
        xor     l         ; A = r3p3r1c3c4c5c2r2
        rrca              ; A = r2r3p3r1c3c4c5c2
        rrca              ; A = c2r2r3p3r1c3c4c5
        xor     c
        and     $67
        xor     c         ; A = r1r2r3c1c2c3c4c5
        ld      l, a      ; L = r1r2r3c1c2c3c4c5

; In-place byte swap permutation
        sbc     hl, de    ; HL < DE ?
        jr      nc, drcs_skip ; skip otherwise
        add     hl, de
        ld      c, (hl)   ; swap contents (HL) and (DE)
        ld      a, (de)
        ld      (hl), a
        ld      a, c
        ld      (de), a
.drcs_skip
        inc     b         ; djnz adjust
        inc     de        ; next address
        ld      a, d
        cp      2*8+72
        jr      nz, drcs_loop ; process next address
        djnz    drcs_next ; process next pass
        ret
#endasm
#endif



#ifdef MODE_128K
#asm
	._ram_page
		defb 0
#endasm
void unpack_RAMn (unsigned char n, unsigned int address, unsigned int destination) {
	ram_address [0] = address;
	ram_page [0] = n;
	ram_destination [0] = destination;

	#asm	
		di
		ld a, (_ram_page)
		ld b, a
		call SetRAMBank
		
		ld hl, (_ram_address)
		ld de, (_ram_destination)
		call depack
		
		ld b, 0
		call SetRAMBank
		ei
	#endasm
}
#else
void unpack (unsigned int address, unsigned int destination) {
	if (address != 0) {
		ram_address [0] = address;
		ram_destination [0] = destination;

		#asm	
			ld hl, (_ram_address)
			ld de, (_ram_destination)
			call depack
			#ifdef COMPRESS_SCREENS
			call drcs_onscreen
			#endif
		#endasm
	}
}
#endif


#ifndef SCR_W
#define SCR_W 15
#endif

#ifndef SCR_H
#define SCR_H 10
#endif

#ifdef COMPRESSED_MAPS
void __FASTCALL__ descomprimir_map ( unsigned char pantalla) {
  #asm
        ld      a, l
        and     a
        ld      b, h
        ld      c, h
        ld      de, map
        ld      hl, fin-1
desc1:  sbc     hl, bc
        ex      de, hl
        ld      c, (hl)
        ex      de, hl
        inc     de
        dec     a
        jp      p, desc1
        ld      de, DMAP_BUFFER+SCR_W*SCR_H-1
        ld      b, $80          ; marker bit
desc2:  ld      a, 256 / 2^DMAP_BITSYMB
desc3:  call    gbit3           ; load DMAP_BITSYMB bits (literal)
        jr      nc, desc3
#if   (DMAP_BITHALF==1)
  #if   (DMAP_BITSYMB==1)
        rrca
        jr      nc, desc4
        xor     a
        call    gbit3
        inc     a
  #else
        rrca                    ; half bit implementation (ie 48 tiles)
        call    c, gbit1
  #endif
#else
        and     a
#endif
desc4:  ld      (de), a         ; write literal
desc5:  dec     e               ; test end of file (map is always 150 bytes)
        ret     z
        call    gbit3           ; read one bit
        rra
        jr      nc, desc2       ; test if literal or sequence
        push    de              ; if sequence put de in stack
        ld      a, 1            ; determine number of bits used for length
desc6:  call    nc, gbit3       ; (Elias gamma coding)
        and     a
        call    gbit3
        rra
        jr      nc, desc6       ; check end marker
        inc     a               ; adjust length
        ld      c, a            ; save lenth to c
        xor     a
        ld      de, SCR_W
        call    gbit3           ; get two bits
        call    gbit3
        jr      z, desc9        ; 00 = 1
        dec     a
        call    gbit3
        jr      z, descb        ; 010 = 15
        bit     2, a
        jr      nz, desc7
#if   (SCR_W>15)
        call    gbit3           ; [011, 100, 101] xx = from 2 to 13
        dec     a
        call    gbit3
        jr      desca
desc7:  call    gbit3           ; [110, 111] xxxxxx = from 14-15, 17-142
        jr      nc, desc7
        cp      SCR_W-14
        sbc     a, -14
#else
  #if   (SCR_W==15)
        add     a, $7c          ; [011, 100, 101] xx = from 2 to 13
        dec     e
desc7:  dec     e               ; [110, 111] xxxxxx = 14 and from 16 to 142
desc8:  call    gbit3
        jr      nc, desc8
        jr      z, descb
        add     a, e
  #else
        call    gbit3           ; [011, 100, 101] xx = from 2 to 11 and from 13 to 14
        call    gbit3
        cp      SCR_W+2
        sbc     a, 2
        jr      desc9
desc7:  call    gbit3           ; [110, 111] xxxxxx = from 15 to 142
        jr      nc, desc7
        add     a, 14
  #endif
#endif
desc9:  inc     a
desca:  ld      e, a
descb:  ld      a, b            ; save b (byte reading) on a
        ld      b, d            ; b= 0 because lddr moves bc bytes
        ex      (sp), hl        ; store source, restore destination
        ex      de, hl          ; HL = destination + offset + 1
        add     hl, de          ; DE = destination
        lddr
        pop     hl              ; restore source address (compressed data)
        ld      b, a            ; restore b register
        inc     e               ; prepare test of end of file
        jr      desc5           ; jump to main loop
#if   (DMAP_BITHALF==1 && DMAP_BITSYMB>1)
gbit1:  sub     $80 - (2^(DMAP_BITSYMB-2))
        defb    $da             ; second part of half bit implementation
#endif
gbit2:  ld      b, (hl)         ; load another group of 8 bits
        dec     hl
gbit3:  rl      b               ; get next bit
        jr      z, gbit2        ; no more bits left?
        adc     a, a            ; put bit in a
        ret
.map    BINARY "mapa_comprimido.bin"
.fin
  #endasm
}
#endif
