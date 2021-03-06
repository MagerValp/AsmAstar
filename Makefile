TARGET=c64
CC=cl65
AS=ca65
LD=ld65
C1541=c1541
CFLAGS=-Oirs -t $(TARGET)
AFLAGS=-t $(TARGET)
LDFLAGS=
GCC=gcc
GCCFLAGS=-O


%.o: %.c
	$(CC) -c $(CFLAGS) $<

%.o: %.s
	$(AS) $(AFLAGS) $<


all: main.prg


OBJS = \
	main.o \
	benchmark.o \
	openqueue.o \
	map.o \
	map_get.o \
	map_distance.o \
	actor.o \
	path.o \
	path_came_from.o \
	path_queued_cost.o \
	editor.o


asmastar.d64: asmastar.prg
	@-rm -f tmp.d64
	@c1541 -format 'asmastar,gp' d64 tmp.d64
	@c1541 tmp.d64 -write asmastar.prg 	   	'asmastar     /mv'
	@mv tmp.d64 $@

asmastar.prg: main.prg
	exomizer sfx sys -m 4096 -q -o $@ $^

main.prg: $(OBJS)
	$(CC) -m asmastar.map -Ln asmastar.lab -C asmastar.cfg -o $@ $(LDFLAGS) $^


clean:
	rm -f *.o
	rm -f asmastar.prg main.prg asmastar.map asmastar.lab
	rm -f asmastar.d64 tmp.d64


distclean: clean
	rm -f *~
