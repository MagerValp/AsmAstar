#include <stdint.h>
#include <stdlib.h>
#include <c64.h>
#include <conio.h>
#include "map.h"
#include "actor.h"
#include "editor.h"


void main(void) {
    VIC.addr = 0x15;
    VIC.bordercolor = 8;
    VIC.bgcolor0 = 8;
    
    map_init();
    actor_init();
    editor_init();
    
    editor_main();
}
