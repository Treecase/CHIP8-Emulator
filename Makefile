
CC=gcc

OBJN=mem. cpu. chip8. display. io. low.

HEAD=$(addprefix src/, $(addsuffix h, $(OBJN)))
OBJS=$(addprefix src/, $(addsuffix c, $(OBJN)))
OBJC=$(addprefix src/objs/, $(addsuffix o, $(OBJN)))



chip8 : src/main.c $(OBJC)
	$(CC) src/main.c $(OBJC) -o chip8 -g -lX11

$(OBJC) : $(OBJS) $(HEAD)
	$(CC) $(OBJS) -c -g -lX11
	mv $(addsuffix o, $(OBJN)) src/objs

clean :
	rm src/objs/* chip8

