#include "map.h"
#include "actor.h"


uint8_t actor_xpos[ACTOR_COUNT];
uint8_t actor_ypos[ACTOR_COUNT];
uint8_t actor_char[ACTOR_COUNT];
uint8_t actor_color[ACTOR_COUNT];


void actor_init(void) {
    actor_xpos[0] = 0;
    actor_ypos[0] = 0;
    actor_char[0] = 'A';
    actor_color[0] = 0;
    actor_xpos[1] = MAP_WIDTH - 1;
    actor_ypos[1] = 0;
    actor_char[1] = 'S';
    actor_color[1] = 10;
    actor_xpos[2] = 0;
    actor_ypos[2] = MAP_HEIGHT - 1;
    actor_char[2] = 'X';
    actor_color[2] = 0;
    actor_xpos[3] = MAP_WIDTH - 1;
    actor_ypos[3] = MAP_HEIGHT - 1;
    actor_char[3] = 'Z';
    actor_color[3] = 10;
}


uint8_t actor_at(uint8_t x, uint8_t y) {
    uint8_t i;
    
    for (i = 0; i < ACTOR_COUNT; ++i) {
        if (actor_xpos[i] == x && actor_ypos[i] == y) {
            return i;
        }
    }
    return ACTOR_NONE;
}