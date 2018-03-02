/*
 * Low level interfacing b/t emu and OS
 * (ie for drawing, IO, etc.)
 *
 */

#ifndef _LOW_H
#define _LOW_H


extern unsigned int G_scale;

void low_initdisplay();

void low_cleanup();

unsigned char low_readkey();

void low_wholeboard();

void low_drawpixel (unsigned int x, unsigned int y, unsigned char on);

void low_update();

void low_buzzer();


#endif

