CC=g++
CFLAGS=-Wall -g
OBJECTS=chip8.o main.o
EXECUTABLE=main
LDFLAGS=-lSDL2

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

chip8.o: chip8.cpp chip8.h
	$(CC) $(CFLAGS) -c $<

main.o: main.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o
