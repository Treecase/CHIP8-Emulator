/*
 * CHIP-8 emu main stuff
 *
 */

#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "chip8.h"
#include "cpu.h"
#include "mem.h"
#include "io.h"
#include "display.h"
#include "low.h"



char g_quit = 0;


/* chip8_args: handle argv */
char *chip8_args (int argc, char *argv[]) {

    char *in_file = NULL;
    for (int i = 1; i < argc; ++i) {
    
        if (!strcmp (argv[i], "-v")
         || !strcmp (argv[i], "--verbose"))
            G_IO_loglevel = IO_VERBOSE;

        else if (!strcmp (argv[i], "-d")
         || !strcmp (argv[i], "--debug"))
            G_IO_loglevel = IO_DEBUG;

        else if (!strcmp (argv[i], "-h")
         || !strcmp (argv[i], "--help")) {
            IO_print_help (argv[0]);
            return NULL;
        }
        else if (!strcmp (argv[i], "-s")
         || !strcmp (argv[i], "--scale")) {
            if (i+1 < argc)
                G_scale = atoi (argv[++i]);
            else
                fatal ("chip8_args - missing scale!\n", argv[i]);
        }

        else if (!in_file)      // not a recognized arg, filename?
            in_file = argv[i];
        else {
            IO_print_usage (argv[0]);
            return NULL;
        }

    }
    if (!in_file)
        fatal ("%s - no filename supplied!\n", argv[0]);
    return in_file;
}

/* chip8_init: initialize memory */
void chip8_init() {

    memset (memory, 0, MEMSIZE);
    mem_loadsprites();

    memset (stack, 0, STACKSIZE);

    srand (time (NULL));

    sp = 0;
    pc = 0x200; // programs begin at byte 512
    for (int i = 0; i < 16; ++i)
        V[i] = 0;
    I = 0;
    delay_timer = sound_timer = 0;

    display_clear();
    low_initdisplay();
}

/* chip8_cleanup: clean up */
void chip8_cleanup() {
    low_cleanup();
}

/* chip8_load: load the program */
void chip8_load (char *fname) {

    int fd = open (fname, O_RDONLY);
    if (fd == -1)
        fatal ("chip8_load - failed to load file '%s'\n", fname);

    int length = lseek (fd, 0, SEEK_END);
    lseek (fd, 0, SEEK_SET);

    if (length > MEMSIZE - 512)
        fatal ("chip8_load - file '%s' is too large! (%i/%i)\n",
            fname, length, MEMSIZE - 512);

    read (fd, memory+512, length);

}

/* chip8_cycle: do an emulation cycle */
void chip8_cycle() {

    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    if (opcode == 0) {
        g_quit = 1;
        return;
    }

    log ("%i: ", pc);
    pc += 2;

    cpu_handle_opcode (opcode);

    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0) {
        low_buzzer();
        sound_timer--;
    }
}

/* chip8_draw: draw the screen */
void chip8_draw() {
    display_update();
}

/* chip8_input: get input */
void chip8_input() {
    IO_keystate();
}

