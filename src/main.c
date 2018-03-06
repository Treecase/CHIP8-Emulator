/*
 * A CHIP-8 emulator
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <time.h>

#include "chip8.h"
#include "display.h"
#include "mem.h"



/* CHIP8 emulator */
int main (int argc, char *argv[]) {

    unsigned long starttime;
    struct timespec tspec;

    char *prog_name = chip8_args (argc, argv);
    if (!prog_name)
        return 1;

    chip8_init();
    chip8_load (prog_name);

    while (!g_quit) {
        clock_gettime (CLOCK_REALTIME, &tspec);
        starttime = tspec.tv_nsec;

        chip8_input();

        chip8_cycle();

        if (G_display_dodraw)
            chip8_draw();

        // TODO handle the timing properly
        do {    // lock cycles @ 60hz
            clock_gettime (CLOCK_REALTIME, &tspec);
        } while (tspec.tv_nsec - starttime < G_speed * 10000000/6.0);
    }

    chip8_cleanup();

    return 0;
}

