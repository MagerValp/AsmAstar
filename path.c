#include <string.h>
#include <stdbool.h>
#include "openqueue.h"
#include "map.h"
#include "actor.h"
#include "path.h"


uint8_t path_x[PATH_MAX];
uint8_t path_y[PATH_MAX];
uint8_t path_cost;
uint8_t path_max_cost = 90;

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
        dir = path_get_came_from(x, y);
        x += dx[dir];
        y += dy[dir];
        if (x == start_x && y == start_y) {
            return len;
        }
    }
}

#define path_add_node(X, Y, C, D) \
    if (path_set_queued_cost_if_lower((X), (Y), (C))) { \
        path_set_came_from((X), (Y), (D)); \
        openqueue_push(map_distance((X), (Y)), (C), (X), (Y)); \
    }

uint8_t path_find(uint8_t start_x, uint8_t start_y, uint8_t new_dest_x, uint8_t new_dest_y) {
    static uint8_t x, y, cost;
    static uint8_t passable_north, passable_east, passable_south, passable_west;
    
    // Set the global map destination variables.
    map_dest_x = new_dest_x;
    map_dest_y = new_dest_y;
    
    // Clear the visitation and cost buffers.
    path_clear_came_from();
    path_clear_queued_cost();
    
    // Start off the open queue with the start node.
    openqueue_init(path_max_cost);
    openqueue_push(map_distance(start_x, start_y), 0, start_x, start_y);
    // Loop as long as there are nodes to explore.
    while (openqueue_size) {
        // Ignore node if we've already visited with a lower cost.
        cost = path_get_queued_cost(openqueue_xpos, openqueue_ypos);
        if (openqueue_cost <= cost) {
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
            passable_north = map_is_passable(openqueue_xpos, y);
            if (passable_north) {
                path_add_node(openqueue_xpos, y, cost, SOUTH);
            }
            // South.
            y = openqueue_ypos + 1;
            passable_south = map_is_passable(openqueue_xpos, y);
            if (passable_south) {
                path_add_node(openqueue_xpos, y, cost, NORTH);
            }
            // West.
            x = openqueue_xpos - 1;
            passable_west = map_is_passable(x, openqueue_ypos);
            if (passable_west) {
                path_add_node(x, openqueue_ypos, cost, EAST);
            }
            // East.
            x = openqueue_xpos + 1;
            passable_east = map_is_passable(x, openqueue_ypos);
            if (passable_east) {
                path_add_node(x, openqueue_ypos, cost, WEST);
            }
            
            // Check which diagonal neightbors should be added to the open
            // list. For diagonals we also check that at least one orthogonal
            // neighbor is passable to avoid walking through diagonal walls.
            cost = openqueue_cost + MAP_COST_DIAG;
            
            // Northeast.
            y = openqueue_ypos - 1;
            if ((passable_north || passable_east) && map_is_passable(x, y)) {
                path_add_node(x, y, cost, SOUTHWEST);
            }
            // Southeast.
            y = openqueue_ypos + 1;
            if ((passable_south || passable_east) && map_is_passable(x, y)) {
                path_add_node(x, y, cost, NORTHWEST);
            }
            // Southwest.
            x = openqueue_xpos - 1;
            if ((passable_south || passable_west) && map_is_passable(x, y)) {
                path_add_node(x, y, cost, NORTHEAST);
            }
            // Northwest.
            y = openqueue_ypos - 1;
            if ((passable_north || passable_west) && map_is_passable(x, y)) {
                path_add_node(x, y, cost, SOUTHEAST);
            }
        }
        
        // Remove the top node from the queue and move on to the next.
        openqueue_delete();
    }
    return 0;
}
