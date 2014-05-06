	.include "macro.i"


	.export _benchmark_init
	.export _benchmark_exit
	.export _benchmark_start
	.export _benchmark_stop
	.export _benchmark_result


pal_frq		=  985248
ntsc_frq	= 1022727
resolution	= 100		; Count hundreds of a second.

timer_a		= $dd04
timer_b		= $dd06
timer_cra	= $dd0e
timer_crb	= $dd0f


	.bss

pal_ntsc:	.res 1		; $80 = PAL, $00 = NTSC.


	.code

_benchmark_exit:
	rts


_benchmark_init:
	sei
	
	lda #$ff
:	cmp $d012
	bne :-
	ldx #9
@waitline:
	lda $d012
:	cmp $d012
	beq :-
	dex
	bne @waitline
	
	lda $d011
	and #$80
	sta pal_ntsc
	
	cli

	bmi @pal
	ldax #ntsc_frq / 100
	bne :+
@pal:
	ldax #pal_frq / 100
:
	stax timer_a
	ldax #$ffff
	stax timer_b
	;jmp _benchmark_start


_benchmark_start:
	; PAL/NTSC, force load, start.
	lda #%00010001
	ora pal_ntsc
	sta timer_cra
	; Count A underflows, force load, start.
	lda #%01010001
	sta timer_crb
	
	rts


_benchmark_stop:
	lda #0
	sta timer_cra
	sta timer_crb
	rts


_benchmark_result:
	lda timer_b + 1
	eor #$ff
	tax
	lda timer_b
	eor #$ff
	rts
