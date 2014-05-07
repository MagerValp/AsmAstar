#ifndef OPENQUEUE_H
#define OPENQUEUE_H


#include <stdint.h>

// The size of the queue.
extern uint8_t openqueue_size;
// Peek at the highest priority item in the queue.
extern uint8_t openqueue_cost;
extern uint8_t openqueue_xpos;
extern uint8_t openqueue_ypos;

// Initialize the queue with a maximum cost.
void __fastcall__ openqueue_init(uint8_t max_cost);
// Push a new item into the queue.
void __fastcall__ openqueue_push(uint8_t estimate,
                                 uint8_t cost,
                                 uint8_t xpos,
                                 uint8_t ypos);
// Delete the item at the top of the queue.
void openqueue_delete(void);


#endif
