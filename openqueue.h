#ifndef OPENQUEUE_H
#define OPENQUEUE_H


#include <stdint.h>

extern uint8_t openqueue_size;
// Peek at the item at top of the queue.
extern uint8_t openqueue_cost;
extern uint8_t openqueue_xpos;
extern uint8_t openqueue_ypos;

void __fastcall__ openqueue_init(uint8_t max_cost);
void __fastcall__ openqueue_push(uint8_t estimate,
                                 uint8_t cost,
                                 uint8_t xpos,
                                 uint8_t ypos);
void openqueue_delete(void);


#endif
