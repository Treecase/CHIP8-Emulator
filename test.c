
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


int main() {

    short data[] = {    0x6004,0xB200,  // jmp 0x204
                        0x1202,         // jmp 0x202
                        };
    char ins[2];

    int fd = open ("PROG", O_WRONLY);
    if (!fd)
        return puts ("error");

    for (int b = 0; b < sizeof(data)/2; ++b) {
        ins[1] = data[b] & 0x00FF;
        ins[0] = data[b] >> 8;
        write (fd, ins, 2);
    }

    close (fd);

    return 0;
}
