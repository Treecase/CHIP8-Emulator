/*
 * CHIP-8 emu main stuff
 *
 */

#ifndef _CHIP8_H
#define _CHIP8_H



extern char g_quit;
extern double G_speed;


char *chip8_args (int argc, char *argv[]);

void chip8_init();

void chip8_cleanup();

void chip8_load (char *fname);

void chip8_cycle();

void chip8_draw();

void chip8_input();


#endif

