#ifndef MAP_H
#define MAP_H


#include <stdint.h>


#define MAP_WIDTH 21
#define MAP_HEIGHT 21
#define MAP_COST_ORTH 5
#define MAP_COST_DIAG 7

extern uint8_t *map_line[];

extern uint8_t map_dest_x;
extern uint8_t map_dest_y;

void map_init(void);
uint8_t __fastcall__ map_get(uint8_t x, uint8_t y);
void map_set(uint8_t x, uint8_t y, uint8_t t);
uint8_t map_distance(uint8_t start_x, uint8_t start_y);

#endif
