#ifndef CHIP8_H
#define CHIP8_H

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE SCREEN_WIDTH*SCREEN_HEIGHT
#define KEY_SIZE 16
#define MEMORY_SIZE 4096


class Chip8 {
public:
  Chip8();  // Constructor declaration
  ~Chip8(); // Destructor declaration

  void emulateCycle();  // Emulates one cycle of the system
  int loadApplication(const char * file); // Loads the program in the memory

  bool drawFlag;  // draw flag for when to update screen
  unsigned char gfx[SCREEN_SIZE]; // Graphics for 64x32-pixels screen
  unsigned char key[KEY_SIZE];    // 16-key hexadecimal keypad (0x0-0xF)

private:
  unsigned short opcode; // 35 Opcodes (2-bytes)
  unsigned char memory[MEMORY_SIZE]; // 1 byte * 4096 = 4KB RAM
  unsigned char V[16];  // 15 8-bit registers (1-byte)
  unsigned short I;     // 16-bit I register for storing memory addresses
  unsigned short pc;    // 16-bit program counter register

  unsigned short stack[16]; // The stack is an array of 16 16-bit values
  unsigned short sp;        // 8-bit stack pointer register

  unsigned char delay_timer;  // 8-bit delay timer register
  unsigned char sound_timer;  // 8-bit sound timer register

  void initialize();  // Clears memory, registers and screen

};

/* Systems Memory Map
  0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
  0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
  0x200-0xFFF - Program ROM and work RAM
*/

#endif
