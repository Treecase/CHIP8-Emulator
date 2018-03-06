/*
 * CHIP-8 IO
 *
 * NOTE - keypad layout:
 *      1 2 3 C
 *      4 5 6 D
 *      7 8 9 E
 *      A 0 B F
 *
 */

#ifndef _IO_H
#define _IO_H


#include <stdio.h>
#include <stdlib.h>
#include <X11/keysym.h>

enum {
    IO_STANDARD,
    IO_VERBOSE,
    IO_DEBUG
};
extern int G_IO_loglevel;

#define error(...)  {fprintf (stderr, "Error: ");\
                    fprintf (stderr, __VA_ARGS__);}
#define fatal(...)  {error (__VA_ARGS__);\
                    exit (-1);}

#define log(...)    IO_log (IO_VERBOSE, __VA_ARGS__)
#define logd(...)   IO_log (IO_DEBUG,   __VA_ARGS__)

/*  0 1 2 3  >  1 2 3 C
    4 5 6 7  >  4 5 6 D
    8 9 A B  >  7 8 9 E
    C D E F  >  A 0 B F */
// keypad keys
#define DEFAULT_KEYBINDS    XK_KP_0,XK_KP_7,XK_KP_8,XK_KP_9,        \
                            XK_KP_4,XK_KP_5,XK_KP_6,XK_KP_1,        \
                            XK_KP_2,XK_KP_3,XK_Right,XK_KP_Decimal, \
                            XK_KP_Multiply,XK_KP_Subtract,XK_KP_Add,\
                            XK_KP_Enter

#define DEFAULT_ALTBINDS    XK_KP_Insert,XK_KP_Home,XK_KP_Up,       \
                            XK_KP_Page_Up,XK_KP_Left,XK_KP_Begin,   \
                            XK_KP_Right,XK_KP_End,XK_KP_Down,       \
                            XK_KP_Page_Down,XK_Right,XK_KP_Delete,  \
                            XK_KP_Multiply,XK_KP_Subtract,XK_KP_Add,\
                            XK_KP_Enter

#define KEYBIND_EXIT        XK_Escape


unsigned char   keypad[16];
extern int      keybinds[16];
extern int      altbinds[16];


unsigned char IO_keydown (unsigned char key);

unsigned char IO_readkey();

void IO_keystate();

void IO_log (int log_lvl, char *format, ...);

void IO_print_usage (char *name);

void IO_print_help (char *name);

#endif

