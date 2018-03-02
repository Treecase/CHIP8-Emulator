/*
 * Display for CHIP-8
 *
 */

#include <string.h>

#include "display.h"
#include "cpu.h"
#include "mem.h"
#include "io.h"
#include "low.h"



char G_display_dodraw = 0;


/* display_draw: draw bytes to the screen
    returns 1 if a byte was turned off */
int display_draw (unsigned char x, unsigned char y, uint16_t bytes) {

    if (x > SCR_W)
        error ("display_draw - x out of range (%i/%i)\n", x, SCR_W);
    if (y > SCR_H)
        error ("display_draw - y out of range (%i/%i)\n", y, SCR_H);

    int collide = 0;
    unsigned char pre;
    G_display_dodraw = 1;

    if (bytes == 0) {
        for (int i = 0; i < 16; ++i)
            for (int b = 0; b < 8; ++b) {
                pre = display[x+b][y+i];
                if ((display[x+b][y+i] ^= 1) == 0 && pre)
                    collide = 1;
            }
        return collide;
    }

    // draw
    for (int i = 0; i < bytes; ++i)
        for (int b = 0; b <= 8; ++b) {
            pre = display[x+b][y+i];
            if ((display[x+b][y+i] ^= (mem_get (I+i) & (1 << (8-b)))) == 0
             && pre)
                collide = 1;
        }

    return collide;
}

/* display_update: update the screen */
void display_update() {

    for (int y = 0; y <= SCR_H; ++y) {
        for (int x = 0; x <= SCR_W; ++x) {
            low_drawpixel (x, y, display[x][y]);
        }
    }
    low_update();
    G_display_dodraw = 0;
}

void display_clear() {
    memset (display, 0, (SCR_W+1)*(SCR_H+1));
    G_display_dodraw = 1;
}

