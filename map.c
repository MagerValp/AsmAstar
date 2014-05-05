#include <string.h>
#include "map.h"


static uint8_t map[MAP_WIDTH * MAP_HEIGHT] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
    0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,0,0,1,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,
    0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,0,0,
    0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,
    0,1,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,
    0,1,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,
    0,1,0,1,1,1,1,1,0,1,0,0,1,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,
    0,1,0,1,1,1,1,1,1,1,0,0,1,0,0,0,0,0,0,0,0,
    0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0
};
uint8_t *map_line[MAP_HEIGHT];


void map_init(void) {
    static uint8_t y;
    
    for (y = 0; y < MAP_HEIGHT; ++y) {
        map_line[y] = &map[y * MAP_WIDTH];
    }
    //memset(map, 0, MAP_WIDTH * MAP_HEIGHT);
}


uint8_t map_get(uint8_t x, uint8_t y) {
    return map_line[y][x];
}

void map_set(uint8_t x, uint8_t y, uint8_t t) {
    map_line[y][x] = t;
}

uint8_t map_distance(uint8_t start_x, uint8_t start_y, uint8_t dest_x, uint8_t dest_y) {
    static uint8_t dx, dy;
    static uint8_t dmin, dmax;
    
    if (start_x > dest_x) {
        dx = start_x - dest_x;
    } else {
        dx = dest_x - start_x;
    }
    if (start_y > dest_y) {
        dy = start_y - dest_y;
    } else {
        dy = dest_y - start_y;
    }
    if (dx > dy) {
        dmax = dx;
        dmin = dy;
    } else {
        dmax = dy;
        dmin = dx;
    }
    return dmin * MAP_COST_DIAG + MAP_COST_ORTH * (dmax - dmin);
}
