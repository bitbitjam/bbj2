; Fast RND (Modified for adding an entropy souce, by ronaldo)
; Got from: http://www.z80.info/pseudo-random.txt
;
; An 8-bit pseudo-random number generator,
; using a similar method to the Spectrum ROM,
; - without the overhead of the Spectrum ROM.
;
; R = random number seed
; an integer in the range [1, 256]
;
; R -> (33*R) mod 257
;
; S = R - 1
; an 8-bit unsigned integer

;; Size: 
;; Time: 20 Microsecs, 80 CPY Cycles

_getRandom_u8::
   ;; No need to recover parameters from stack. This function uses,
   ;;  __z88dk_fastcall convention so its unique 8-bits parameter is in L.
u8_rand_seed::
   ld   a, #00      ;; [2] A = Seed
   ld   c, a        ;; [1] C = Copy of A
   
   xor  l           ;; [1] Use parameter in L = Entropy source (mesh up bits)

   rrca             ;; [1] A = A * 32 (without taking Carry into account)
   rrca             ;; [1]
   rrca             ;; [1]
   xor  #0x1F       ;; [2]

   add  c           ;; [1] A  = 32A + A = 33A
   sbc  #0xFF       ;; [2] A += 1 - Carry

   ld (u8_rand_seed + 1), a ;; [4] Save new random number as seed
   ld   l, a        ;; [1] L = Return parameter
   ret              ;; [3] Return
