#include <iostream>
#include "chip8.h"
using namespace std;

/* Chip-8 Font Set (0-F) representation in hex */
unsigned char chip8_fontset[80] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8()
{
	// empty
}

Chip8::~Chip8()
{
	// empty
}

void Chip8::initialize()
{

  opcode = 0;   // Reset current Opcode
  I = 0;        // Reset I register
  pc = 0x200;   // Set PC to start at 0x200 (Program Start Adress)
  sp = 0;       // Reset stack pointer

  /* Reset Timers */
  delay_timer = 0;
  sound_timer = 0;

	for (int i = 0; i < SCREEN_SIZE; i++)
    gfx[i] = 0;     // Clear Display

  for (int i = 0; i < KEY_SIZE; i++)
    key[i] = 0;     // Clear Keypad

  for (int i = 0; i < MEMORY_SIZE; i++)
    memory[i] = 0;  // Clear Memory

  for (int i = 0; i < 16; i++)
  {
    V[i] = 0;       // Clear Registers
    stack[i] = 0;   // Clear Stack
  }

  for (int i = 0; i < 80; i++)
    memory[i] = chip8_fontset[i];  // Store Font Set to Memory (0x000-0x050)

  /* Clear screen once */
  drawFlag = true;

  // srand (time(NULL));

}

void Chip8::emulateCycle()
{
  /* Fetch next Opcode from memory */
  unsigned short opcode = memory[pc] << 8 | memory[pc + 1];  // opcode is 2 bytes, memory stores 1 byte

  /* Decode Opcode */
  switch (opcode & 0xF000)  // Switch on MSB
  {
    case 0x0000:
      switch (opcode & 0x000F)
      {
        case 0x0000:  // 00E0 - CLS: Clear the display.
          // Execute opcode
        break;

        case 0x000E:  // 00EE - RET: Return from a subroutine.
          // Execute opcode
        break;

        default:
          printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
      }
    break;

    case 0x1000:  // 1nnn - JP addr: Jump to location nnn.
      // Execute opcode
    break;

    case 0x2000:  // 2nnn - CALL addr: Call subroutine at nnn.
      // Execute opcode
    break;

    case 0x3000:  // 3xkk - SE Vx, byte: Skip next instruction if Vx = kk.
      // Execute opcode
    break;

    case 0x4000:  // 4xkk - SNE Vx, byte: Skip next instruction if Vx != kk.
      // Execute opcode
    break;

    case 0x5000:  // 5xy0 - SE Vx, Vy: Skip next instruction if Vx = Vy.
      // Execute opcode
    break;

    case 0x6000:  // 6xkk - LD Vx, byte: Set Vx = kk.
      // Execute opcode
    break;

    case 0x7000:  // 7xkk - ADD Vx, byte: Set Vx = Vx + kk.
      // Execute opcode
    break;

    case 0x8000:
      switch (opcode & 0x000F)
      {
        case 0x0000:  // 8xy0 - LD Vx, Vy: Set Vx = Vy.
          // Execute opcode
        break;

        case 0x0001:  // 8xy1 - OR Vx, Vy: Set Vx = Vx OR Vy.
          // Execute opcode
        break;

        case 0x0002:  // 8xy2 - AND Vx, Vy: Set Vx = Vx AND Vy.
          // Execute opcode
        break;

        case 0x0003:  // 8xy3 - XOR Vx, Vy: Set Vx = Vx XOR Vy.
          // Execute opcode
        break;

        case 0x0004:  //  8xy4 - ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry.
          // Execute opcode
        break;

        case 0x0005:  //  8xy5 - SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow.
          // Execute opcode
        break;

        case 0x0006:  //  8xy6 - SHR Vx {, Vy}: Set Vx = Vx SHR 1.
          // Execute opcode
        break;

        case 0x0007:  // 8xy7 - SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow.
          // Execute opcode
        break;

        case 0x000E:  // 8xyE - SHL Vx {, Vy}: Set Vx = Vx SHL 1.
          // Execute opcode
        break;

        default:
          printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
      }
    break;

    case 0x9000:  // 9xy0 - SNE Vx, Vy: Skip next instruction if Vx != Vy.
      // Execute opcode
    break;

    case 0xA000:  // Annn - LD I, addr: Set I = nnn.
      // Execute opcode
    break;

    case 0xB000:  // Bnnn - JP V0, addr: Jump to location nnn + V0.
      // Execute opcode
    break;

    case 0xC000:  // Cxkk - RND Vx, byte: Set Vx = random byte AND kk.
      // Execute opcode
    break;

    case 0xD000:  // Dxyn - DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
      // Execute opcode
    break;

    case 0xE000:
      switch (opcode & 0x00FF)
      {
        case 0x009E:  // Ex9E - SKP Vx: Skip next instruction if key with the value of Vx is pressed.
          // Execute opcode
        break;

        case 0x00A1:  // ExA1 - SKNP Vx: Skip next instruction if key with the value of Vx is not pressed.
          // Execute opcode
        break;

        default:
          printf ("Unknown opcode [0xE000]: 0x%X\n", opcode);
      }
    break;

    case 0xF000:
      switch (opcode & 0x00FF)
      {
        case 0x0007:  // Fx07 - LD Vx, DT: Set Vx = delay timer value.
          // Execute opcode
        break;

        case 0x000A:  // Fx0A - LD Vx, K: Wait for a key press, store the value of the key in Vx.
          // Execute opcode
        break;

        case 0x0015:  // Fx15 - LD DT, Vx: Set delay timer = Vx.
          // Execute opcode
        break;

        case 0x0018:  // Fx18 - LD ST, Vx: ST is set equal to the value of Vx.
          // Execute opcode
        break;

        case 0x001E:  // Fx1E - ADD I, Vx: Set I = I + Vx.
          // Execute opcode
        break;

        case 0x0029:  // Fx29 - LD F, Vx: Set I = location of sprite for digit Vx.
          // Execute opcode
        break;

        case 0x0033:  // Fx33 - LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2.
          // Execute opcode
        break;

        case 0x0055:  // Fx55 - LD [I], Vx: Store registers V0 through Vx in memory starting at location I.
          // Execute opcode
        break;

        case 0x0065:  // Fx65 - LD Vx, [I]: Read registers V0 through Vx from memory starting at location I.
          // Execute opcode
        break;

        default:
          printf ("Unknown opcode [0xF000]: 0x%X\n", opcode);
      }
    break;

    default:
      printf ("Unknown opcode: 0x%X\n", opcode);
  }

}

int Chip8::loadApplication(const char * file)
{
	initialize();

  /* Open File */
  FILE *fptr;
  long file_size;

  if ((fptr = fopen(file,"rb")) == NULL)
  {
    printf("Error! opening program\n");
    return -1;
  }

  /* Check file size */
  fseek(fptr, 0, SEEK_END); // seek to end of file (non-portable !)
  file_size = ftell(fptr);
  fseek(fptr, 0, SEEK_SET); // seek back to beginning of file
  printf("Filesize: %ld bytes\n", file_size);

  /* Allocate memory to contain the whole file */
  char *buffer = (char*) malloc(sizeof(char)*file_size);
  if (buffer == NULL)
  {
    fprintf(stderr, "Memory error! Could not allocate %ld bytes of memory\n", file_size);
    fclose(fptr);
    return -1;
  }

  /* Copy the file into the buffer */
  size_t res = fread(buffer, 1, file_size, fptr);
  if (res != file_size)
  {
    fprintf(stderr, "Read error! Could not read %ld bytes of memory\n", file_size);
    fclose(fptr);
    free(buffer);
    return -1;
  }

  /* Copy buffer to Chip8 memory */
  if (file_size > MEMORY_SIZE - 512) // If file cannot fit into memory
  {
    fprintf(stderr, "Error! Program of %ld bytes too big for memory \n", file_size);
    fclose(fptr);
    free(buffer);
    return -1;
  }
  for(int i = 0; i < file_size; i++)
    memory[i + 512] = buffer[i];  // Start program at (0x200 == 512)

  /* Close File, Free Buffer */
  fclose(fptr);
  free(buffer);

  return 0;
}
