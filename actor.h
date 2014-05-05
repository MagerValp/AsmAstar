#ifndef ACTOR_H
#define ACTOR_H


#include <stdint.h>

#define ACTOR_COUNT 4
#define ACTOR_NONE 255

extern uint8_t actor_xpos[];
extern uint8_t actor_ypos[];
extern uint8_t actor_char[];
extern uint8_t actor_color[];

void actor_init(void);
uint8_t actor_at(uint8_t x, uint8_t y);


#endif
