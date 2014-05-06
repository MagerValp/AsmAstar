	.include "macro.i"
	.include "map.i"
	
	
	.export _map_distance
	
	.import _map_dest_x
	.import _map_dest_y
	
	.import popa
	.importzp sp
	
	
	.code

; uint8_t __fastcall__ map_distance(uint8_t start_x, uint8_t start_y);
_map_distance:
	sec			; abs(start_y - map_dest_y)
	sbc _map_dest_y
	bcs :+
	eor #$ff
	;clc
	adc #1
:
	sta @dmin		; Default to dy being smaller than dx.
	
	jsr popa		; abs(start_x - map_dest_y)
	sec
	sbc _map_dest_x
	bcs :+
	eor #$ff
	;clc
	adc #1
:
	cmp @dmin		; Is dx smaller than dy?
	bcs :+
	tax
	lda @dmin
	stx @dmin
:
	sec			; orth = dmax - dmin
	sbc @dmin
	
	; Multiply by MAP_COST_ORTH.
	.assert MAP_COST_ORTH = 5, error, "expected MAP_COST_ORTH to be 5"
	
	sta @one		; Multiply by 4 and add 1.
	asl
	asl
	clc
@one = * + 1
	adc #$5e
	sta @dist

	; Multiply by MAP_COST_DIAG.
	.assert MAP_COST_DIAG = 7, error, "expected MAP_COST_DIAG to be 7"
	
	lda @dmin		; Multiplay by 8 and subtract 1.
	asl
	asl
	asl
	sec
@dmin = * + 1
	sbc #$5e
	clc
@dist = * + 1
	adc #$5e
	ldx #0
	rts
