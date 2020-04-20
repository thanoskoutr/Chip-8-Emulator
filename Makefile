CC=g++
CFLAGS=-Wall -g

main: chip8.o main.o
	$(CC) -o main main.o chip8.o

clean:
	rm chip8.o main.o
