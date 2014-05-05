#ifndef PATH_H
#define PATH_H


#include <stdint.h>

#define PATH_MAX 32

extern uint8_t path_x[PATH_MAX];
extern uint8_t path_y[PATH_MAX];

uint8_t path_find(uint8_t start_x, uint8_t start_y, uint8_t dest_x, uint8_t dest_y);


#endif
