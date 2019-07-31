;;
;; Sets the random number seed for geting u8 random values
;;
;; Size: 
;; Time: 7 Microsecs, 28 CPU Cycles

.globl u8_rand_seed

_setRandomSeed_u8::
   ;; No need to recover parameters from stack. This function uses,
   ;;  __z88dk_fastcall convention so its unique 8-bits parameter is in L.

   ld   a, l                ;; [1] A = new seed (Passed as parameter in L)
   ld (u8_rand_seed + 1), a ;; [4] Save parameter as new random seed

   ret                      ;; [3] Return
