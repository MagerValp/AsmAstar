	.include "macro.i"


	.export _openqueue_size
	.export _openqueue_init
	.export _openqueue_push
	.export _openqueue_delete
	.export _openqueue_cost
	.export _openqueue_xpos
	.export _openqueue_ypos
	
	.importzp sp
	.import incsp3
	
	
	.bss

	.align 128

prio:		.res 128
cost:		.res 128
xpos:		.res 128
ypos:		.res 128
size:		.res 1
max_cost:	.res 1

; Exported names.
_openqueue_size = 	size
_openqueue_cost = 	cost
_openqueue_xpos = 	xpos
_openqueue_ypos = 	ypos


	.code

; Initialize an empty heap.
_openqueue_init:
	clc
	adc #1
	sta max_cost
	lda #0
	sta size
	rts


; Swap node at x with node at y.
  .macro swap
	lda prio,x
	sta @tempe
	lda prio,y
	sta prio,x
@tempe = * + 1
	lda #$5e
	sta prio,y

	lda cost,x
	sta @tempc
	lda cost,y
	sta cost,x
@tempc = * + 1
	lda #$5e
	sta cost,y

	lda xpos,x
	sta @tempx
	lda xpos,y
	sta xpos,x
@tempx = * + 1
	lda #$5e
	sta xpos,y

	lda ypos,x
	sta @tempy
	lda ypos,y
	sta ypos,x
@tempy = * + 1
	lda #$5e
	sta ypos,y
  .endmacro

; Push a value into the tree.
_openqueue_push:
	inc $d020
	ldx size
	; If the heap is full, overwrite the last node.
	bpl :+
	dex
	stx size
:	; Push arguments to the bottoms of the parallel heaps.
	sta ypos,x
	ldy #0
	lda (sp),y
	sta xpos,x
	iny
	lda (sp),y
	sta cost,x
	iny
	clc
	adc (sp),y
	; Ignore if estimate + cost is larger than or equal to max.
	cmp max_cost
	bcs @done
	sta prio,x
	
	inc size

	; Set Y to the parent node = (X - 1) / 2.
	txa
	sec
	sbc #1
	lsr
	tay
@heapify:
	; If X is at the top of the heap, we're done.
	cpx #0
	beq @done
	; If X is larger than its parent Y, we're done.
	lda prio,x
	cmp prio,y
	bcc :+
@done:
	dec $d020
	jmp incsp3
:	; Swap X with its parent Y.
	swap
	; Set X to point at its parent.
	tya
	tax
	; Set Y to the new parent node.
	sec
	sbc #1
	lsr
	tay
	jmp @heapify


; Pop the lowest value from the tree.
_openqueue_delete:
	dec $d020
	dec size
	beq @done
	ldy size
	; Remove the item at the bottom of the tree and move it to the top of
	; the tree.
	lda prio,y
	sta prio
	lda cost,y
	sta cost
	lda xpos,y
	sta xpos
	lda ypos,y
	sta ypos
	; Heapify the tree.
	ldx #0
	stx @current_node
@heapify:
	; Left child is at x * 2 + 1.
	txa
	asl
	;clc		; X is always < 128
	adc #1
	tay
	; Check if we're at the bottom of the tree.
	cpy size
	bcc :+
@done:
	inc $d020
	rts
:	; Check if left child is smaller.
	lda prio,y
	cmp prio,x
	; If not skip to checking the right child.
	bcs @check_right
	; Since it was smaller let X point to the left child.
	tya
	tax
@check_right:
	; Right child is at x * 2 + 2, or left + 1.
	iny
	; Check if we're at the bottom of the tree.
	cpy size
	bcs @check_swap
	; Check if right child is smaller.
	lda prio,y
	cmp prio,x
	bcs @check_swap
	tya
	tax

@check_swap:
	; If either child was smaller X is different from the current node.
@current_node = * + 1
	cpx #$5e		; Self mod.
	; If not we're done.
	beq @done
	
	; Swap parent with child.
	ldy @current_node
	swap
	; Let the child be the new current node.
	stx @current_node
	; Repeat.
	jmp @heapify
