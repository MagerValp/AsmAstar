	.include "macro.i"
	.include "map.i"
	
	
	.export _path_clear_queued_cost
	.export _path_set_queued_cost_if_lower
	
	.import popa, popax
	.importzp ptr1
	
	
	.bss

	.align 256

cost_buf:	.res MAP_WIDTH * MAP_HEIGHT

	.rodata

	.align 64

queued_cost_lo:
  .repeat MAP_HEIGHT, I
	.byte <(cost_buf + MAP_WIDTH * I)
  .endrepeat
queued_cost_hi:
  .repeat MAP_HEIGHT, I
	.byte >(cost_buf + MAP_WIDTH * I)
  .endrepeat


	.code

; void path_clear_queued_cost(void);
_path_clear_queued_cost:
	lda #$ff
	ldx #0
:	sta cost_buf,x
	sta cost_buf + MAP_WIDTH * MAP_HEIGHT - 256,x
	inx
	bne :-
	rts


; uint8_t __fastcall__ path_set_queued_cost_if_lower(uint8_t x, uint8_t y, uint8_t cost)
_path_set_queued_cost_if_lower:
	sta @cost
	
	jsr popax
	tay
	lda queued_cost_lo,y
	sta ptr1
	lda queued_cost_hi,y
	sta ptr1 + 1
	txa
	tay
@cost = * + 1
	lda #$5e
	cmp (ptr1),y
	bcc :+
	lda #0
	tax
	rts
:	sta (ptr1),y
	ldx #$ff
	rts
