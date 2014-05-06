#include <string.h>
#include <stdbool.h>
#include <conio.h>
#include "openqueue.h"
#include "map.h"
#include "actor.h"
#include "path.h"


#define OPENQUEUE_COLOR 9

uint8_t path_x[PATH_MAX];
uint8_t path_y[PATH_MAX];
uint8_t path_cost;
uint8_t path_max_cost = 90;

static uint8_t came_from[MAP_WIDTH * MAP_HEIGHT];
static uint8_t queued_cost[MAP_WIDTH * MAP_HEIGHT];

enum {
    NORTH = 0,
    NORTHEAST,
    EAST,
    SOUTHEAST,
    SOUTH,
    SOUTHWEST,
    WEST,
    NORTHWEST,
    NOT_VISITED = 0xff
};

static int8_t dx[8] = {
    0, 1, 1, 1, 0, -1, -1, -1
};
static int8_t dy[8] = {
    -1, -1, 0, 1, 1, 1, 0, -1
};

uint8_t reconstruct_path(uint8_t start_x, uint8_t start_y) {
    static uint8_t len, dir;
    static uint8_t x, y;
    
    len = 0;
    x = map_dest_x;
    y = map_dest_y;
    for (;;) {
        if (len >= PATH_MAX) {
            return 0;
        }
        path_x[len] = x;
        path_y[len] = y;
        ++len;
        dir = came_from[y * MAP_WIDTH + x];
        x += dx[dir];
        y += dy[dir];
        if (x == start_x && y == start_y) {
            return len;
        }
    }
}

void set_came_from(uint8_t x, uint8_t y, uint8_t from) {
    gotoxy(x + 2, y + 2);
    switch (from) {
        case NORTH:
        cputc(94);
        break;
        case EAST:
        cputc('>');
        break;
        case SOUTH:
        cputc('v');
        break;
        case WEST:
        cputc('<');
        break;
        case NORTHEAST:
        cputc(0xae);
        break;
        case SOUTHEAST:
        cputc(0xbd);
        break;
        case SOUTHWEST:
        cputc(0xad);
        break;
        case NORTHWEST:
        cputc(0xb0);
        break;
    }
    came_from[y * MAP_WIDTH + x] = from;
}

bool is_passable(uint8_t x, uint8_t y) {
    if (x < MAP_WIDTH &&
        y < MAP_HEIGHT &&
        map_get(x, y) == 0) {
        return true;
    }
    return false;
}

#define not_visited(X, Y) came_from[(Y) * MAP_WIDTH + (X)] == NOT_VISITED

#define get_queued_cost(X, Y) queued_cost[(Y) * MAP_WIDTH + (X)]
#define set_queued_cost(X, Y, C) queued_cost[(Y) * MAP_WIDTH + (X)] = (C)

uint8_t path_find(uint8_t start_x, uint8_t start_y, uint8_t new_dest_x, uint8_t new_dest_y) {
    static uint8_t x, y, cost;
    static uint8_t passable_north, passable_east, passable_south, passable_west;
    
    // Set the global map destination variables.
    map_dest_x = new_dest_x;
    map_dest_y = new_dest_y;
    
    memset(came_from, NOT_VISITED, sizeof(came_from));
    memset(queued_cost, 255, sizeof(queued_cost));
    
    textcolor(OPENQUEUE_COLOR);
    
    // Start off the open queue with the start node.
    openqueue_init(path_max_cost);
    openqueue_push(map_distance(start_x, start_y), 0, start_x, start_y, NOT_VISITED);
    // Loop as long as there are nodes to explore.
    while (openqueue_size) {
        // Avoid visiting nodes twice.
        if (not_visited(openqueue_xpos, openqueue_ypos)) {
            set_came_from(openqueue_xpos, openqueue_ypos, openqueue_dir);
            // Mark node as visited with the direction from which we came.
            // Check if we've arrived at our destination.
            if (openqueue_xpos == map_dest_x && openqueue_ypos == map_dest_y) {
                // Store the final path cost.
                path_cost = openqueue_cost;
                // Reconstruct the path and return the length.
                return reconstruct_path(start_x, start_y);
            }
            
            // Check which orthogonal neightbors should be added to the open
            // list. We only add nodes that are passable and which have a
            // lower cost than any previously added nodes at the same
            // coordinates.
            cost = openqueue_cost + MAP_COST_ORTH;
            
            // North.
            y = openqueue_ypos - 1;
            passable_north = is_passable(openqueue_xpos, y);
            if (passable_north) {
                if (get_queued_cost(openqueue_xpos, y) > cost) { 
                    set_queued_cost(openqueue_xpos, y, cost);
                    openqueue_push(map_distance(openqueue_xpos, y),
                                   cost,
                                   openqueue_xpos,
                                   y,
                                   SOUTH);
                }
            }
            // South.
            y = openqueue_ypos + 1;
            passable_south = is_passable(openqueue_xpos, y);
            if (passable_south) {
                if (get_queued_cost(openqueue_xpos, y) > cost) { 
                    set_queued_cost(openqueue_xpos, y, cost);
                    openqueue_push(map_distance(openqueue_xpos, y),
                                   cost,
                                   openqueue_xpos,
                                   y,
                                   NORTH);
                }
            }
            // East.
            x = openqueue_xpos + 1;
            passable_east = is_passable(x, openqueue_ypos);
            if (passable_east) {
                if (get_queued_cost(x, openqueue_ypos) > cost) { 
                    set_queued_cost(x, openqueue_ypos, cost);
                    openqueue_push(map_distance(x, openqueue_ypos),
                                   cost,
                                   x,
                                   openqueue_ypos,
                                   WEST);
                }
            }
            // West.
            x = openqueue_xpos - 1;
            passable_west = is_passable(x, openqueue_ypos);
            if (passable_west) {
                if (get_queued_cost(x, openqueue_ypos) > cost) { 
                    set_queued_cost(x, openqueue_ypos, cost);
                    openqueue_push(map_distance(x, openqueue_ypos),
                                   cost,
                                   x,
                                   openqueue_ypos,
                                   EAST);
                }
            }
            
            // Check which diagonal neightbors should be added to the open
            // list. For diagonals we also check that at least one orthogonal
            // neighbor is passable to avoid walking through diagonal walls.
            cost = openqueue_cost + MAP_COST_DIAG;
            
            // Northeast.
            x = openqueue_xpos + 1;
            y = openqueue_ypos - 1;
            if ((passable_north || passable_east) && is_passable(x, y)) {
                if (get_queued_cost(x, y) > cost) { 
                    set_queued_cost(x, y, cost);
                    openqueue_push(map_distance(x, y),
                                   cost,
                                   x,
                                   y,
                                   SOUTHWEST);
                }
            }
            // Southeast.
            y = openqueue_ypos + 1;
            if ((passable_south || passable_east) && is_passable(x, y)) {
                if (get_queued_cost(x, y) > cost) { 
                    set_queued_cost(x, y, cost);
                    openqueue_push(map_distance(x, y),
                                   cost,
                                   x,
                                   y,
                                   NORTHWEST);
                }
            }
            // Southwest.
            x = openqueue_xpos - 1;
            if ((passable_south || passable_west) && is_passable(x, y)) {
                if (get_queued_cost(x, y) > cost) { 
                    set_queued_cost(x, y, cost);
                    openqueue_push(map_distance(x, y),
                                   cost,
                                   x,
                                   y,
                                   NORTHEAST);
                }
            }
            // Northwest.
            y = openqueue_ypos - 1;
            if ((passable_north || passable_west) && is_passable(x, y)) {
                if (get_queued_cost(x, y) > cost) { 
                    set_queued_cost(x, y, cost);
                    openqueue_push(map_distance(x, y),
                                   cost,
                                   x,
                                   y,
                                   SOUTHEAST);
                }
            }
        }
        
        gotoxy(0, 0);
        cprintf("%d ", openqueue_size);
        
        // Remove the top node from the queue and move on to the next.
        openqueue_delete();
    }
    return 0;
}
