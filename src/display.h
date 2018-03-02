/*
 * Display for CHIP-8
 *
 */

#ifndef _DISPLAY_H
#define _DISPLAY_H


#include <stdint.h>


#define SCR_W   64
#define SCR_H   32


unsigned char display[SCR_W][SCR_H];

extern char G_display_dodraw;


int display_draw (unsigned char x, unsigned char y, uint16_t bytes);

void display_update();

void display_clear();


#endif

