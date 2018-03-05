/*
 * CHIP-8 memory module
 *
 */

#include "mem.h"
#include "io.h"



/* mem_loadsprites: load the default sprites */
void mem_loadsprites() {

    unsigned char sprites[] =
    {   0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
        0x20, 0x60, 0x20, 0x20, 0x70,   // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
        0xF0, 0x80, 0xF0, 0x80, 0x80,   // F
        0x00                            // end
    };
    for (int i = 0; sprites[i] != 0; ++i)
        memory[i] = sprites[i];
}

/* mem_set: set a byte in memory */
unsigned char mem_set (uint16_t byte, unsigned char data) {
    if (byte < 0x200)
        error ("mem_set - overwriting ROM! (%hu)\n", byte);
    if (byte > 0xFFF)
        fatal ("mem_set - address %u is out of range!\n", byte);
    return memory[byte] = data;
}

/* mem_get: get a byte from memory */
unsigned char mem_get (uint16_t byte) {
    if (byte > 0xFFF)
        fatal ("mem_get - address %u is out of range!\n", byte);
    return memory[byte];
}

/* mem_sprite: return the index of the font for char c */
uint16_t mem_sprite (unsigned char c) {
    return c*5;
}

/* FIXME TEMP mem_print: print memory */
void mem_print() {

    puts ("MEMORY");
    for (int i = 0; i < 0x200 && memory[i] != 0; ++i) {
        printf ("%.3X ", i);
        for (int b = 0; b < 8; ++b)
            putchar ((memory[i] & (1 << (7-b)))? '1' : '0');
        putchar ('\n');
    }
    puts ("DONE MEMORY\n");
}

