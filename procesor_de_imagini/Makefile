CC=gcc
CFLAGS=-Wall

.PHONY: all run clean

all: procesor_de_imagini

run: procesor_de_imagini
	./procesor_de_imagini

clean:
	rm *.o procesor_de_imagini

procesor_de_imagini: main.o
	$(CC) main.o -o procesor_de_imagini

main.o: main.c
	$(CC) $(CFLAGS) -c main.c