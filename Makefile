CC = cc

.PHONY: all

all: trawniczek

trawniczek: main.o gardener.o input.o lawnholder.o output.o picasso8.o
	$(CC) -o trawniczek main.o gardener.o input.o output.o lawnholder.o picasso8.o

main.o: main.c gardener.h input.h lawnholder.h output.h
	$(CC) -o main.o -c main.c

gardener.o: gardener.c gardener.h
	$(CC) -o gardener.o -c gardener.c

input.o: input.c input.h
	$(CC) -o input.o -c input.c

lawnholder.o: lawnholder.c lawnholder.h 
	$(CC) -o lawnholder.o -c lawnholder.c

output.o: output.c output.h 
	$(CC) -o output.o -c output.c

picasso8.o: picasso8.c picasso8.h
	$(CC) -o picasso8.o -c picasso8.c


.PHONY: clean

clean:
	-rm *.o trawniczek
