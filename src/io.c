/*
 * CHIP-8 keypad
 *
 * NOTE - keypad layout:
 *      1 2 3 C
 *      4 5 6 D
 *      7 8 9 E
 *      A 0 B F
 *
 */

#include <stdarg.h>

#include "chip8.h"
#include "io.h"
#include "low.h"


int keybinds[16] = { DEFAULT_KEYBINDS };

int G_IO_loglevel = IO_STANDARD;


/* IO_keydown: returns 1 if key is pressed, else 0 */
unsigned char IO_keydown (unsigned char key) {

    if (key > 16)
        fatal ("IO_keydown - key %hhu is out of range (16)\n", key);

    return keypad[key];

}

/* IO_readkey: wait until a key is pressed, return pressed key */
unsigned char IO_readkey() {

    unsigned char keyval;

    keyval = low_readkey();

    keypad[keyval] = 1;

    return keyval;
}

/* IO_keystate: get the keypad state at the moment, nonblocking */
void IO_keystate() {
    low_wholeboard();
}

/* IO_log: general log messaging */
void IO_log (int log_lvl, char *format, ...) {

    va_list ap;
    va_start (ap, format);

    if (G_IO_loglevel >= log_lvl)
        vprintf (format, ap);

    va_end (ap);
}

/* IO_print_usage: print usage information */
void IO_print_usage (char *name) {

    printf ("Usage: %s FILENAME [OPTIONS]\n", name);
    printf ("Try '%s --help' for more information.\n", name);
    g_quit = 1;
}

/* IO_print_help: print help */
void IO_print_help (char *name) {

    IO_print_usage (name);
    puts ("FILENAME is the path to a CHIP-8 program");
    printf ("Example: %s -v --scale 10 PONG\n\n", name);
    puts (" -v, --verbose\tverbose logging, prints"
            "actions and byte number");
    puts (" -d, --debug\tdebug logging, verbose + opcodes");
    puts (" -h, --help\tdisplay this help and exit");
    puts (" -s, --scale N\tscale display by N times");
}

