	.include "macro.i"
	.include "map.i"
	
	
	.export _path_clear_came_from
	.export _path_get_came_from
	.export _path_set_came_from
	
	.import popa, popax
	


OPENQUEUE_COLOR =	9
PLOT_CHARS =		1


	.bss

	.align 256

visit_buf:	.res MAP_WIDTH * MAP_HEIGHT

	.rodata

	.align 64

came_from_lo:
  .repeat MAP_HEIGHT, I
	.byte <(visit_buf + MAP_WIDTH * I)
  .endrepeat
came_from_hi:
  .repeat MAP_HEIGHT, I
	.byte >(visit_buf + MAP_WIDTH * I)
  .endrepeat


	.code

; void path_clear_came_from(void);
_path_clear_came_from:
	lda #$ff
	ldx #0
:	sta visit_buf,x
	sta visit_buf + MAP_WIDTH * MAP_HEIGHT - 256,x
	inx
	bne :-
	rts


; uint8_t __fastcall__ path_get_came_from(uint8_t x, uint8_t y);
_path_get_came_from:
	tay
	lda came_from_lo,y
	sta @came_ptr
	lda came_from_hi,y
	sta @came_ptr + 1
	jsr popa
	tax
@came_ptr = * + 1
	lda @came_ptr,x
	ldx #0
	rts
	

; void __fastcall__ path_set_came_from(uint8_t x, uint8_t y, uint8_t from);
_path_set_came_from:
	sta @dir
  .ifdef PLOT_CHARS
	tax
	lda char,x
	sta @c
  .endif
	
	jsr popax
	tay
  .ifdef PLOT_CHARS
	lda screen_line_lo,y
	sta @screen_ptr
	lda screen_line_hi,y
	sta @screen_ptr + 1
	lda color_line_lo,y
	sta @color_ptr
	lda color_line_hi,y
	sta @color_ptr + 1
@c = * + 1
	lda #$5e
@screen_ptr = * + 1
	sta $5e1f,x
	lda #OPENQUEUE_COLOR
@color_ptr = * + 1
	sta $5e1f,x
  .endif
	
	lda came_from_lo,y
	sta @came_ptr
	lda came_from_hi,y
	sta @came_ptr + 1
@dir = * + 1
	lda #$5e
@came_ptr = * + 1
	sta @came_ptr,x
	rts

	
	.rodata

	.align 64

screen_offset = 2 * 40 + 2

screen_line_lo:
  .repeat 25, I
	.byte <($0400 + screen_offset + I * 40)
  .endrepeat
screen_line_hi:
  .repeat 25, I
	.byte >($0400 + screen_offset + I * 40)
  .endrepeat

color_line_lo:
  .repeat 25, I
	.byte <($d800 + screen_offset + I * 40)
  .endrepeat
color_line_hi:
  .repeat 25, I
	.byte >($d800 + screen_offset + I * 40)
  .endrepeat

char:
	.byte $1e	; ^
	.byte $6e
	.byte $3e	; >
	.byte $7d
	.byte $16	; v
	.byte $6d
	.byte $3c	; <
	.byte $70
