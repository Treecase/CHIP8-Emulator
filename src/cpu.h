/*
 * CHIP-8 CPU
 *
 */

#ifndef _CPU_H
#define _CPU_H


#include <stdint.h>



#define STACKSIZE       16


// registers (V0-VF)
// note: VF is a flag register
extern unsigned char    V[16];

// index register
extern uint16_t         I;

// call stack + pointer
extern uint16_t         stack[STACKSIZE];
extern unsigned char    sp;
extern uint16_t         pc;

// timers
extern unsigned char    delay_timer;
extern unsigned char    sound_timer;


void cpu_handle_opcode (uint16_t code);

#endif

