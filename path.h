#ifndef PATH_H
#define PATH_H


#include <stdint.h>

#define PATH_MAX 50

extern uint8_t path_x[PATH_MAX];
extern uint8_t path_y[PATH_MAX];
extern uint8_t path_cost;
extern uint8_t path_max_cost;

void path_clear_came_from(void);
uint8_t __fastcall__ path_get_came_from(uint8_t x, uint8_t y);
void __fastcall__ path_set_came_from(uint8_t x, uint8_t y, uint8_t from);

void path_clear_queued_cost(void);
uint8_t __fastcall__ path_set_queued_cost_if_lower(uint8_t x, uint8_t y, uint8_t cost);

uint8_t path_find(uint8_t start_x, uint8_t start_y, uint8_t dest_x, uint8_t dest_y);


#endif
