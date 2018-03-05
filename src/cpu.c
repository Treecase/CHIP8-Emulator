/*
 * CHIP-8 CPU
 *
 */

#include <stdio.h>

#include "cpu.h"
#include "mem.h"
#include "display.h"
#include "io.h"


void unknown_opcode (uint16_t code);


uint16_t        I, stack[STACKSIZE], pc;
unsigned char   V[16], sp, delay_timer, sound_timer;



/* handle_opcode: interpret an opcode */
void cpu_handle_opcode (uint16_t code) {

    logv ("opcode %.4hX\n", code);

    uint16_t        NNN;
    unsigned char   NN, N, x, y;
    NNN =  code & 0x0FFF      ;
    NN  =  code & 0x00FF      ;
    N   =  code & 0x000F      ;
    x   = (code & 0x0F00) >> 8;
    y   = (code & 0x00F0) >> 4;

    switch (code & 0xF000) {

    case 0x0000:
        switch (code) {
        // clear screen
        case 0x00E0:
            display_clear();
            log ("clear screen\n");
            break;
        // return from subroutine
        case 0x00EE:
            pc = stack[--sp];
            log ("ret %u\n", pc);
            break;
        default:
            unknown_opcode (code);
            break;
        }
        break;

    // 1NNN: goto NNN
    case 0x1000:
        pc = NNN-2;
        log ("jmp %u\n", NNN);
        break;

    // 2NNN: call func @ NNN
    case 0x2000:
        stack[sp++] = pc;
        pc = NNN-2;
        log ("call %u\n", NNN);
        break;

    // 3xNN: skip next instruction if Vx = NN
    case 0x3000:
        log ("je V%x (%hhu) = %hhu : %s\n", x, V[x], NN,
            V[x] == NN? "yes" : "no");
        if (V[x] == NN)
            pc += 2;
        break;

    // 4xNN: skip next instruction if Vx != NN
    case 0x4000:
        log ("jne V%x (%hhu) = %hhu : %s\n", x, V[x], NN,
            V[x] != NN? "yes" : "no");
        if (V[x] != NN)
            pc += 2;
        break;

    // 5xy0: skip next instruction if Vx == Vy
    case 0x5000:
        if (N) {
            unknown_opcode (code);
            break;
        }
        log ("je V%x (%hhu) = V%x (%hhu) : %s\n", x, V[x], y, V[y],
            V[x] == V[y]? "yes" : "no");
        if (V[x] == V[y])
            pc += 2;
        break;

    // 6xNN: set Vx = NN
    case 0x6000:
        V[x] = NN;
        log ("set V%x = %hhu\n", x, NN);
        break;

    // 7xNN: add NN to Vx
    case 0x7000:
        V[x] += NN;
        log ("V%x += %hhu (=%hhu)\n", x, NN, V[x]);
        break;

    // 8NNN: inter-register ops
    case 0x8000:
        switch (N) {
        
        // 8xy0: set Vx = Vy
        case 0x0000:
            V[x] = V[y];
            log ("set V%x = V%x (=%hhu)\n", x, y, V[x]);
            break;

        // 8xy1: set Vx = Vx | Vy
        case 0x0001:
            V[x] |= V[y];
            log ("V%x | V%x (=%hhu)\n", x, y, V[x]);
            break;

        // 8xy2: set Vx = Vx & Vy
        case 0x0002:
            V[x] &= V[y];
            log ("V%x & V%x (=%hhu)\n", x, y, V[x]);
            break;

        // 8xy3: set Vx = Vx ^ Vy
        case 0x0003:
            V[x] ^= V[y];
            log ("V%x ^ V%x (=%hhu)\n", x, y, V[x]);
            break;

        // 8xy4: set Vx = Vx + Vy, VF = carry
        case 0x0004:
            V[0xF] = (V[y] > V[x])? 1 : 0;
            V[x] += V[y];
            log ("V%x + V%x (=%hhu, Vf=%hhu)\n", x, y, V[x], V[0xF]);
            break;

        // 8xy5: set Vx = Vx - Vy, VF = !carry
        case 0x0005:
            V[0xF] = (V[x] > V[y])? 1 : 0;  // >= ?
            V[x] -= V[y];
            log ("V%x - V%x (=%hhu,Vf=%hhu)\n", x, y, V[x], V[0xF]);
            break;

        // 8xy6: set Vx = Vy >> 1, VF = LSB of Vy
        case 0x0006:
            V[0xF] = (V[x] & 1);
            V[x] >>= 1;
            log ("V%x >> 1 (=%hhu,Vf=%hhu)\n", x, V[x], V[0xF]);
            break;

        // 8xy7: set Vx = Vy - Vx, VF = !carry
        case 0x0007:
            V[0xF] = (V[x] > V[y])? 0 : 1;  // >= ?
            V[x] = V[y] - V[x];
            log ("V%x - V%x (=%hhu,Vf=%hhu)\n", y, x, V[x], V[0xF]);
            break;

        // 8xyE: set Vx = Vy << 1, VF = LSB of Vy
        case 0x000E:
            V[0xF] = V[x] >> 7;
            V[x] <<= 1;
            log ("V%x << 1 (=%hhu,Vf=%hhu)\n", x, V[x], V[0xF]);
            break;

        default:
            unknown_opcode (code);
            break;
        }
        break;

    // 9xy0: skip next instruction if Vx != Vy
    case 0x9000:
        log ("jne V%x (%hhu) = V%x (%hhu) : %s\n", x, V[x], y, V[y],
            V[x] != V[y]? "yes" : "no");
        if (V[x] != V[y])
            pc += 2;
        break;

    // ANNN: set I = NNN
    case 0xA000:
        log ("set I = %hu\n", NNN);
        I = NNN;
        break;

    // BNNN: jump to NNN + V0
    case 0xB000:
        pc = NNN + V[0]-2;
        log ("jmp %hu+V0 (%hhu) (=%hu)\n", NNN, V[0], pc+2);
        break;

    // CxNN: set Vx = randbyte & NN
    case 0xC000:
        V[x] = (rand() % 255) & NN;
        log ("set V%x = rand & %hhu (=%hhu)\n", x, NN, V[x]);
        break;

    // DxyN: draw bytes I -> I+N to the screen at coords Vx,Vy
    case 0xD000:{
        V[0xF] = display_draw (V[x], V[y], N);
        log ("draw %hhu bytes @ %hhu,%hhu (Vf=%hhu)\n", N, V[x],V[y],
            V[0xF]);
        break;}

    case 0xE000:
        switch (NN) {

        // Ex9E: skip next instruction if key Vx is pressed
        case 0x009E:
            log ("jmp %X pressed : %s\n", V[x],
                IO_keydown (V[x])? "yes" : "no");
            if (IO_keydown (V[x]))
                pc += 2;
            break;

        // ExA1: skip next instruction if key Vx is not pressed
        case 0x00A1:
            log ("jmp %X not pressed : %s\n", V[x],
                !IO_keydown (V[x])? "yes" : "no");
            if (!IO_keydown (V[x]))
                pc += 2;
            break;

        // unknown
        default:
            unknown_opcode (code);
            break;
        }
        break;

    case 0xF000:
        switch (NN) {

        // Fx07: set Vx = delay timer value
        case 0x0007:
            V[x] = delay_timer;
            log ("set V%x = dt (=%hhu)\n", x, V[x]);
            break;

        // Fx0A: wait for keypress, store value of key in Vx
        case 0x000A:
            V[x] = IO_readkey();
            log ("V%x = keyp (=%X)\n", x, V[x]);
            break;

        // Fx15: set delay timer = Vx
        case 0x0015:
            delay_timer = V[x];
            log ("set dt = V%x (=%hhu)\n", x, V[x]);
            break;

        // Fx18: set sound timer = Vx
        case 0x0018:
            sound_timer = V[x];
            log ("set st = V%x (=%hhu)\n", x, V[x]);
            break;

        // Fx1E: set I = I + Vx, Vf = range overflow
        case 0x001E:
            V[0xF] = (I+V[x] > 0xFFF)? 1 : 0;
            I += V[x];
            log ("I + V%x (=%u)\n", x, I);
            break;

        // Fx29: set I = location of sprite Vx
        case 0x0029:
            I = mem_sprite (V[x]);
            log ("set I = spr%X (=%u)\n", V[x], I);
            break;

        // Fx33: binary coded decimal representation
        // of Vx in I, I+1, I+2
        case 0x0033:
            mem_set (I,    V[x] / 100      ); 
            mem_set (I+1, (V[x] / 10 ) % 10);
            mem_set (I+2, (V[x] % 100) % 10);
            log ("BCT of %u @ %u\n", V[x], I);
            break;

        // Fx55: copy V0->Vx into memory[I]->memory[I+x]
        case 0x0055:
            for (int i = 0; i <= x; ++i)
                mem_set (I+i, V[i]);
            I += x+1;
            log ("cp V0 -> V%x to mem @ %u\n", x, I);
            break;

        // Fx65: copy memory[I]->memory[I+x] into V0->Vx
        case 0x0065:
            for (int i = 0; i < x; ++i)
                V[i] = mem_get (I+i);
            I += x+1;
            log ("cp mem (%u) to V0 -> V%x\n", I, x);
            break;

        }
        break;

    // unknown
    default:
        break;
    }
}

/* unknown_opcode: prints unknown opcode message */
void unknown_opcode (uint16_t code) {
    error ("handle_opcode - unknown opcode %.4hX\n", code);
}

