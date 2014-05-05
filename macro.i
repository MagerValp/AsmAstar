; load A/X
 .macro ldax arg
	.if (.match (.left (1, {arg}), #))	; immediate mode
	lda #<(.right (.tcount ({arg})-1, {arg}))
	ldx #>(.right (.tcount ({arg})-1, {arg}))
	.else					; assume absolute or zero page
	lda arg
	ldx 1+(arg)
	.endif
 .endmacro

; store A/X
 .macro stax arg
	sta arg
	stx 1+(arg)
 .endmacro	

; convert ascii to screencodes
 .macro screencode str
	.repeat .strlen(str), I
		.if ((.strat(str, I) & $60) = $20)
			.byte .strat(str, I)
		.endif
		.if ((.strat(str, I) & $e0) = $40)
			.byte .strat(str, I) & $1f
		.endif
		.if ((.strat(str, I) & $e0) = $60) .or ((.strat(str, I) & $e0) = $c0)
			.byte .strat(str, I) & $1f | $40
		.endif
	.endrepeat
 .endmacro


; define chained raster IRQ handlers
 .macro IRQ_DEFINE name, line
	
	.ident(.concat(name, "_line")) = line
	.ident(name):
	
	sta @savea
	stx @savex
	sty @savey
	
 .endmacro
	
 .macro IRQ_NEXT next
	
	ldax #.ident(next)
	stax $fffe
	lda #(.ident(.concat(next, "_line")))
	sta $d012
	
	inc $d019

@savey = * + 1
	ldy #$5e
@savex = * + 1
	ldx #$5e
@savea = * + 1
	lda #$5e
	rti
	
 .endmacro

 .macro IRQ_STABILIZE

	lda $dc04
	eor #7
	sta * + 4
	bpl * + 2
	lda #$a9
	lda #$a9
	lda $eaa5

 .endmacro

; debugging
DEBUG = 0

 .if DEBUG
	.macro DEBUG_DEC_D020
	dec $d020
	.endmacro
	.macro DEBUG_INC_D020
	inc $d020
	.endmacro
	.macro DEBUG_RESET_D020
	lda #0
	sta $d020
	.endmacro
 .else
	.macro DEBUG_DEC_D020
	.endmacro
	.macro DEBUG_INC_D020
	.endmacro
	.macro DEBUG_RESET_D020
	.endmacro
 .endif
