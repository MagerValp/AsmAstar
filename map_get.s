	.include "macro.i"
	
	
	.export _map_get
	
	.import _map_line
	
	.import popa
	.importzp sp
	
	
	.code

; uint8_t __fastcall__ map_get(uint8_t x, uint8_t y);
_map_get:
	asl
	tax
	lda _map_line,x
	sta @map_line_ptr
	lda _map_line + 1,x
	sta @map_line_ptr + 1
	jsr popa
	tax
@map_line_ptr = * + 1
	lda $5e1f,x
	ldx #0
	rts
