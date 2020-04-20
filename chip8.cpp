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
          for (int i = 0; i < SCREEN_SIZE; i++)
            gfx[i] = 0x0;
          drawFlag = true;
          pc += 2;
        break;

        case 0x000E:  // 00EE - RET: Return from a subroutine.
          sp--;
          pc = stack[sp];
          pc += 2;
        break;

        default:
          printf ("Unknown opcode [0x0000]: 0x%X\n", opcode);
      }
    break;

    case 0x1000:  // 1nnn - JP addr: Jump to location nnn.
      pc = opcode & 0x0FFF;
    break;

    case 0x2000:  // 2nnn - CALL addr: Call subroutine at nnn.
      stack[sp] = pc;
      sp++;
      pc = opcode & 0x0FFF;
    break;

    case 0x3000:  // 3xkk - SE Vx, byte: Skip next instruction if Vx = kk.
      if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        pc += 4;
      else
        pc += 2;
    break;

    case 0x4000:  // 4xkk - SNE Vx, byte: Skip next instruction if Vx != kk.
      if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        pc += 4;
      else
        pc += 2;
    break;

    case 0x5000:  // 5xy0 - SE Vx, Vy: Skip next instruction if Vx = Vy.
      if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
        pc += 4;
      else
        pc += 2;
    break;

    case 0x6000:  // 6xkk - LD Vx, byte: Set Vx = kk.
      V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
      pc += 2;
    break;

    case 0x7000:  // 7xkk - ADD Vx, byte: Set Vx = Vx + kk.
      V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
      pc += 2;
    break;

    case 0x8000:
      switch (opcode & 0x000F)
      {
        case 0x0000:  // 8xy0 - LD Vx, Vy: Set Vx = Vy.
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
          pc += 2;
        break;

        case 0x0001:  // 8xy1 - OR Vx, Vy: Set Vx = Vx OR Vy.
          V[(opcode & 0x0F00) >> 8] |= V[(opcode & 0x00F0) >> 4];
          pc += 2;
        break;

        case 0x0002:  // 8xy2 - AND Vx, Vy: Set Vx = Vx AND Vy.
          V[(opcode & 0x0F00) >> 8] &= V[(opcode & 0x00F0) >> 4];
          pc += 2;
        break;

        case 0x0003:  // 8xy3 - XOR Vx, Vy: Set Vx = Vx XOR Vy.
          V[(opcode & 0x0F00) >> 8] ^= V[(opcode & 0x00F0) >> 4];
          pc += 2;
        break;

        case 0x0004:  //  8xy4 - ADD Vx, Vy: Set Vx = Vx + Vy, set VF = carry.
          if (V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4] > 0xFF) // Check for overflow
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] += V[(opcode & 0x00F0) >> 4];
          pc += 2;
        break;

        case 0x0005:  //  8xy5 - SUB Vx, Vy: Set Vx = Vx - Vy, set VF = NOT borrow.
          if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])  // check Vx-Vy
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] -= V[(opcode & 0x00F0) >> 4];
          pc += 2;
        break;

        case 0x0006:  //  8xy6 - SHR Vx {, Vy}: Set Vx = Vx SHR 1.
          if ((V[(opcode & 0x0F00) >> 8] & 0x1) == 1) // Check if LSB == 1
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] >>= 1;  // divide by 2 == bitwise right shift with 1
          pc += 2;
        break;

        case 0x0007:  // 8xy7 - SUBN Vx, Vy: Set Vx = Vy - Vx, set VF = NOT borrow.
          if (V[(opcode & 0x0F00) >> 8] < V[(opcode & 0x00F0) >> 4])  // check Vy-Vx
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
          pc += 2;
        break;

        case 0x000E:  // 8xyE - SHL Vx {, Vy}: Set Vx = Vx SHL 1.
          if (V[(opcode & 0x0F00) >> 8] >> 7 == 1) // Check if MSB == 1
            V[0xF] = 1;
          else
            V[0xF] = 0;
          V[(opcode & 0x0F00) >> 8] <<= 1;  // multiply by 2 == bitwise left shift with 1
          pc += 2;
        break;

        default:
          printf ("Unknown opcode [0x8000]: 0x%X\n", opcode);
      }
    break;

    case 0x9000:  // 9xy0 - SNE Vx, Vy: Skip next instruction if Vx != Vy.
      if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
        pc += 4;
      else
        pc += 2;
    break;

    case 0xA000:  // Annn - LD I, addr: Set I = nnn.
      I = opcode & 0x0FFF;
      pc += 2;
    break;

    case 0xB000:  // Bnnn - JP V0, addr: Jump to location nnn + V0.
      pc = (opcode & 0x0FFF) + V[0];
    break;

    case 0xC000:  // Cxkk - RND Vx, byte: Set Vx = random byte AND kk.
      V[(opcode & 0x0F00) >> 8] = (rand() % 0xFF) & (opcode & 0x00FF);  // generate random number between 0-255
      pc += 2;
    break;

    case 0xD000:  // Dxyn - DRW Vx, Vy, nibble: Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    {
      /* A sprite is a set of 8 pixels = 8 bits = 1 byte,
       * Each row of 8 pixels is read as bit-coded starting from memory location I;
       * I value doesn't change after the execution of this instruction.
       * VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
         and to 0 if that doesn't happen */

      unsigned short x = V[(opcode & 0x0F00) >> 8];
      unsigned short y = V[(opcode & 0x00F0) >> 4];
      unsigned short height = opcode & 0x000F;
      unsigned short sprite;

      V[0xF] = 0;
      for (int yLine = 0; yLine < height; yLine++)  // read for n-byte sprites
      {
        sprite = memory[I + yLine];  // sprite represents 1 hex value e.g. 0xF2 (= 1 byte = 8 bits = 8 pixels)
        for (int xLine = 0; xLine < 8; xLine++) // each row is 8 pixels
        {
          if ((sprite & (0x80 >> xLine)) != 0 ) // For each bit of the sprite that is 1, 0x80 = 1000 0000
          {
            if (gfx[((y + yLine) * SCREEN_WIDTH) + (x + xLine)] == 1) // Check if the pixel on the display is set to 1
              V[0xF] = 1;
            gfx[((y + yLine) * SCREEN_WIDTH) + (x + xLine)] ^= 1; // Set the pixel value by using XOR
          }
        }
      }
      drawFlag = true;
      pc += 2;
    }
    break;

    case 0xE000:
      switch (opcode & 0x00FF)
      {
        case 0x009E:  // Ex9E - SKP Vx: Skip next instruction if key with the value of Vx is pressed.
          if (key[V[(opcode & 0x0F00) >> 8]] != 0)
            pc += 4;
          else
            pc += 2;
        break;

        case 0x00A1:  // ExA1 - SKNP Vx: Skip next instruction if key with the value of Vx is not pressed.
          if (key[V[(opcode & 0x0F00) >> 8]] == 0)
            pc += 4;
          else
            pc += 2;
        break;

        default:
          printf ("Unknown opcode [0xE000]: 0x%X\n", opcode);
      }
    break;

    case 0xF000:
      switch (opcode & 0x00FF)
      {
        case 0x0007:  // Fx07 - LD Vx, DT: Set Vx = delay timer value.
          V[(opcode & 0x0F00) >> 8] = delay_timer;
          pc += 2;
        break;

        case 0x000A:  // Fx0A - LD Vx, K: Wait for a key press, store the value of the key in Vx.
        {
          bool isPressed = false;
          for (int i = 0; i < KEY_SIZE; i++)
          {
            if (key[i] != 0)
            {
              V[(opcode & 0x0F00) >> 8] = key[i]; // or Vx = i
              isPressed = true;
            }
          }
          if (!isPressed) // If no key pressed, skip this cycle and try again
            return;
          pc += 2;
        }
        break;

        case 0x0015:  // Fx15 - LD DT, Vx: Set delay timer = Vx.
          delay_timer = V[(opcode & 0x0F00) >> 8];
          pc += 2;
        break;

        case 0x0018:  // Fx18 - LD ST, Vx: ST is set equal to the value of Vx.
          sound_timer = V[(opcode & 0x0F00) >> 8];
          pc += 2;
        break;

        case 0x001E:  // Fx1E - ADD I, Vx: Set I = I + Vx.
          if (I + V[(opcode & 0x0F00) >> 8] > 0xFFF) // Check for overflow, undocumented feature
            V[0xF] = 1;
          else
            V[0xF] = 0;
          I += V[(opcode & 0x0F00) >> 8];
          pc += 2;
        break;

        case 0x0029:  // Fx29 - LD F, Vx: Set I = location of sprite for digit Vx.
          I = V[(opcode & 0x0F00) >> 8] * 0x5;  // Each character in font takes 5 bytes of memory
          pc += 2;
        break;

        case 0x0033:  // Fx33 - LD B, Vx: Store BCD representation of Vx in memory locations I, I+1, and I+2.
          memory[I]   = (V[(opcode & 0x0F00) >> 8] / 100);       // hundreds
          memory[I+1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;   // tens
          memory[I+2] = (V[(opcode & 0x0F00) >> 8]) % 10;        // ones
          pc += 2;
        break;

        case 0x0055:  // Fx55 - LD [I], Vx: Store registers V0 through Vx in memory starting at location I.
          for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
            memory[I + i] = V[i];
          /* On the original interpreter, when the operation is done, I = I + X + 1 (I is incremented).
             In SCHIP, I is left unmodified. */
					I += ((opcode & 0x0F00) >> 8) + 1;
          pc += 2;
        break;

        case 0x0065:  // Fx65 - LD Vx, [I]: Read registers V0 through Vx from memory starting at location I.
          for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
            V[i] = memory[I + i];
          /* On the original interpreter, when the operation is done, I = I + X + 1 (I is incremented).
             In SCHIP, I is left unmodified. */
					I += ((opcode & 0x0F00) >> 8) + 1;
          pc += 2;
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
  unsigned long file_size;

  if ((fptr = fopen(file,"rb")) == NULL)
  {
    printf("Error! opening program\n");
    return 1;
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
    return 1;
  }

  /* Copy the file into the buffer */
  size_t res = fread(buffer, 1, file_size, fptr);
  if (res != file_size)
  {
    fprintf(stderr, "Read error! Could not read %ld bytes of memory\n", file_size);
    fclose(fptr);
    free(buffer);
    return 1;
  }

  /* Copy buffer to Chip8 memory */
  if (file_size > MEMORY_SIZE - 512) // If file cannot fit into memory
  {
    fprintf(stderr, "Error! Program of %ld bytes too big for memory \n", file_size);
    fclose(fptr);
    free(buffer);
    return 1;
  }
  for(unsigned int i = 0; i < file_size; i++)
    memory[i + 512] = buffer[i];  // Start program at (0x200 == 512)

  /* Close File, Free Buffer */
  fclose(fptr);
  free(buffer);

  return 0;
}
