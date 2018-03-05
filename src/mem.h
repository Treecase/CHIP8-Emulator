/*
 * CHIP-8 memory module
 *
 */

#ifndef _MEM_H
#define _MEM_H

#include <stdint.h>



// 4k memory
#define MEMSIZE 4096


unsigned char   memory[MEMSIZE];


void mem_loadsprites();

unsigned char mem_set (uint16_t byte, unsigned char data);

unsigned char mem_get (uint16_t byte);

uint16_t mem_sprite (unsigned char c);

void mem_print();


#endif

