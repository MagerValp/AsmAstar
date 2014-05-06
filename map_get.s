	.include "macro.i"
	.include "map.i"
	
	
	.export _map_get
	.export _map_is_passable
	
	.import _map_line
	
	.import popa, incsp1
	
	
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

; bool __fastcall__ map_is_passable(uint8_t x, uint8_t y);
_map_is_passable:
	cmp #MAP_HEIGHT
	bcc :+
	ldx #0
	txa
	jmp incsp1
:
	asl
	tay
	lda _map_line,y
	sta @map_line_ptr
	lda _map_line + 1,y
	sta @map_line_ptr + 1
	
	jsr popa
	cmp #MAP_WIDTH
	bcc :+
@no:
	ldx #0
	txa
	rts
:
	tax
@map_line_ptr = * + 1
	lda $5e1f,x
	bne @no
	ldx #$ff
	txa
	rts
