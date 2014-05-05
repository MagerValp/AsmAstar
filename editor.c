#include <stdint.h>
#include <conio.h>
#include <time.h>
#include "map.h"
#include "actor.h"
#include "path.h"
#include "editor.h"


#define TILE_EMPTY ' '
#define TILE_BLOCKED '*'
#define MAP_COLOR 11
#define PATH_COLOR 7
#define BORDER_COLOR 2


static uint8_t cursor_x;
static uint8_t cursor_y;
static uint8_t cursor_state;
static uint8_t cursor_dir;

static uint8_t editor_actor;

enum {
    CURS_RIGHT = 0,
    CURS_DOWN,
    CURS_LEFT,
    CURS_UP
};


void editor_init(void) {
    clrscr();
    textcolor(BORDER_COLOR);
    revers(0);
    gotoxy(1, 1);
    cputc(CH_ULCORNER);
    chline(MAP_WIDTH);
    cputc(CH_URCORNER);
    cvlinexy(1, 2, MAP_HEIGHT);
    cvlinexy(MAP_HEIGHT + 2, 2, MAP_HEIGHT);
    gotoxy(1, MAP_WIDTH + 2);
    cputc(CH_LLCORNER);
    chline(MAP_WIDTH);
    cputc(CH_LRCORNER);
    
    textcolor(1);
    revers(1);
    cputsxy(25, 0, " play with a* ");
    revers(0);
    cputsxy(25, 2, "crsr to move");
    cputsxy(25, 3, "space boulder");
    cputsxy(25, 4, "1: select A");
    cputsxy(25, 5, "2: select S");
    cputsxy(25, 6, "3: select X");
    cputsxy(25, 7, "4: select Z");
    cputsxy(25, 8, "return to find");
    
    cursor_x = MAP_WIDTH / 2;
    cursor_y = MAP_HEIGHT / 2;
    cursor_state = 0;
    cursor_dir = 0;
    
    editor_actor = 0;
}

void editor_draw_line(uint8_t y) {
    static uint8_t x;
    static uint8_t *line;
    
    gotoxy(2, y + 2);
    line = map_line[y];
    for (x = 0; x < MAP_WIDTH; ++x) {
        cputc(*line++ ? TILE_BLOCKED : TILE_EMPTY);
    }
    for (x = 0; x < ACTOR_COUNT; ++x) {
        if (actor_ypos[x] == y) {
            gotox(actor_xpos[x] + 2);
            textcolor(actor_color[x]);
            cputc(actor_char[x]);
            textcolor(MAP_COLOR);
        }
    }
}

void editor_draw_map(void) {
    static uint8_t y;
    
    textcolor(MAP_COLOR);
    for (y = 0; y < MAP_HEIGHT; ++y) {
        editor_draw_line(y);
    }
}

void editor_draw_map_at(uint8_t x, uint8_t y) {
    gotoxy(x + 2, y + 2);
    cputc(map_get(x, y) ? TILE_BLOCKED : TILE_EMPTY);
}

void editor_draw_actor(uint8_t a) {
    gotoxy(actor_xpos[a] + 2, actor_ypos[a] + 2);
    textcolor(actor_color[a]);
    cputc(actor_char[a]);
}

void editor_draw_cursor(void) {
    uint8_t a;
    
    cursor_state ^= 1;
    revers(cursor_state);
    a = actor_at(cursor_x, cursor_y);
    if (a == ACTOR_NONE) {
        editor_draw_map_at(cursor_x, cursor_y);
    } else {
        editor_draw_actor(a);
    }
    revers(0);
}

void editor_cursor_down(void) {
    if (cursor_y < MAP_HEIGHT - 1) {
        ++cursor_y;
    }
}

void editor_cursor_up(void) {
    if (cursor_y) {
        --cursor_y;
    }
}

void editor_cursor_right(void) {
    if (cursor_x < MAP_WIDTH - 1) {
        ++cursor_x;
    }
}

void editor_cursor_left(void) {
    if (cursor_x) {
        --cursor_x;
    }
}

typedef void (*editor_move_func_t)(void);

static editor_move_func_t editor_move[4] = {
    editor_cursor_right,
    editor_cursor_down,
    editor_cursor_left,
    editor_cursor_up
};

void editor_select_actor(uint8_t a) {
    if (a < ACTOR_COUNT) {
        editor_actor = a;
    }
}

void editor_path_find(void) {
    static uint8_t len;
    static uint8_t step;
    
    if (actor_at(cursor_x, cursor_y) == ACTOR_NONE) {
        if (!map_get(cursor_x, cursor_y)) {
            len = path_find(actor_xpos[editor_actor], actor_ypos[editor_actor],
                            cursor_x, cursor_y);
            textcolor(PATH_COLOR);
            for (step = 0; step < len; ++step) {
                gotoxy(path_x[step] + 2, path_y[step] + 2);
                cputc('W');
            }
            cgetc();
            if (len) {
                actor_xpos[editor_actor] = path_x[0];
                actor_ypos[editor_actor] = path_y[0];
            }
            editor_draw_map();
        }
    }
}


void editor_main(void) {
    static uint16_t now;
    static uint8_t key;
    
    editor_draw_map();
    for (;;) {
        textcolor(MAP_COLOR);
        editor_draw_cursor();
        now = (uint16_t)clock();
        while (!kbhit()) {
            if ((uint16_t)clock() - now >= 32) {
                now = (uint16_t)clock();
                editor_draw_cursor();
            }
        }
        if (cursor_state) {
            editor_draw_cursor();
        }
        key = cgetc();
        switch(key) {
            case CH_CURS_DOWN:
            editor_cursor_down();
            //cursor_dir = CURS_DOWN;
            break;
            
            case CH_CURS_UP:
            editor_cursor_up();
            //cursor_dir = CURS_UP;
            break;
            
            case CH_CURS_RIGHT:
            editor_cursor_right();
            //cursor_dir = CURS_RIGHT;
            break;
            
            case CH_CURS_LEFT:
            editor_cursor_left();
            //cursor_dir = CURS_LEFT;
            break;
            
            case ' ':
            if (actor_at(cursor_x, cursor_y) == ACTOR_NONE) {
                map_set(cursor_x, cursor_y, map_get(cursor_x, cursor_y) ^ 1);
                editor_draw_map_at(cursor_x, cursor_y);
                editor_move[cursor_dir]();
            }
            break;
            
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            editor_select_actor(key - '1');
            break;
            
            case CH_ENTER:
            editor_path_find();
            break;
        }
    }
}
