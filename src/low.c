/*
 * Low level interfacing b/t emu and OS
 * (ie for drawing, IO, etc.)
 *
 */ 
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "low.h"
#include "io.h"
#include "display.h"
#include "chip8.h"



Display         *conn = NULL;
Window          window;
GC              white;
GC              black;
unsigned int    G_scale = 1;


/* low_initdisplay: initialize the display */
void low_initdisplay() {

    conn = XOpenDisplay (NULL);
    if (!conn)
        fatal ("low_initdisplay - failed to open X connection!\n");

    unsigned long c_black = BlackPixel (conn, DefaultScreen (conn));
    unsigned long c_white = WhitePixel (conn, DefaultScreen (conn));

    window = XCreateSimpleWindow (conn, XDefaultRootWindow (conn),
        0,0, SCR_W*G_scale, SCR_H*G_scale, 0, c_black, c_black);
    XMapWindow (conn, window);

    XSelectInput (conn, window, StructureNotifyMask | KeyPressMask
        | KeyReleaseMask);


    white = XCreateGC (conn, window, 0, NULL);
    black = XCreateGC (conn, window, 0, NULL);

    XSetForeground (conn, white, c_white);
    XSetForeground (conn, black, c_black);

    XStoreName (conn, window, "CHIP-8");

    // wait for window to be mapped
    XEvent e;
    while (1) {
        XNextEvent (conn, &e);
        if (e.type == MapNotify)
            break;
    }

    logv ("screen: %ix%i\n", SCR_W*G_scale, SCR_H*G_scale);
}

/* low_cleanup: clean up */
void low_cleanup() {
    XFreeGC (conn, white);
    XFreeGC (conn, black);
    XCloseDisplay (conn);
    conn = NULL;
}

/* low_readkey: read input */
unsigned char low_readkey() {

    char firstpressed = -1;
    char sym[20];
    KeySym keysym;
    XEvent e;
    while (firstpressed < 0) {
        XNextEvent (conn, &e);
        switch (e.type) {
        case KeyPress:
            XLookupString ((XKeyEvent *)&e, sym, 20, &keysym, NULL);
            for (int i = 0; i < 16; ++i) {
                if (keysym == keybinds[i]) {
                    keypad[i] = 1;
                    if (firstpressed < 0)
                        firstpressed = i;
                }
            }
            if (keysym == KEYBIND_EXIT)
                return g_quit = 1;
            XFlush (conn);
            break;
        }
    }
    return firstpressed;
}

/* low_wholeboard: read the state of the keypad */
void low_wholeboard() {

    char map[32];
    XQueryKeymap (conn, map);
    KeyCode code;

    for (int i = 0; i <= 0xF; ++i) {
        code = XKeysymToKeycode (conn, keybinds[i]);
        keypad[i] = (map[code/8] & (1 << (code%8)))? 1 : 0;
    }
    code = XKeysymToKeycode (conn, KEYBIND_EXIT);
    g_quit = (map[code/8] & (1 << (code%8)))? 1 : 0;
}

/* low_drawpixel: draw a pixel */
void low_drawpixel (unsigned int x, unsigned int y,
 unsigned char on) {

    if (x > SCR_W)
        fatal ("low_drawpixel - x (%i) out of range (%i)\n",
            x, SCR_W);
    if (y > SCR_H)
        fatal ("low_drawpixel - y (%i) out of range (%i)\n",
            y, SCR_H);

    x -= 1;
    y -= 1;
    x *= G_scale;
    y *= G_scale;

    GC gc = on? white : black;

    for (int i = 0; i <= G_scale; ++i)
        for (int j = 0; j <= G_scale; ++j)
            XDrawPoint (conn, window, gc, x+i, y+j);
}

/* low_update: update the screen*/
void low_update() {
    XFlush (conn);
}

/* low_buzzer: sound the buzzer */
void low_buzzer() {
    XBell (conn, 100);
}

