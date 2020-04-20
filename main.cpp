#include <iostream>
#include "chip8.h"
using namespace std;

int main (int argc, char **argv)
{
  Chip8 myChip8;

  myChip8.key[0] = 'A';
  cout << "myChip8.key[0] = " << myChip8.key[0] << endl;


  if (myChip8.loadApplication(argv[1]) != 0)
    return 1;

  for (int i = 0; i < SCREEN_SIZE; i++)
  {
    cout << myChip8.gfx[i];
  }
  cout << endl;

  unsigned short originalHEX = 0x8124;
  unsigned short bitAND;
  unsigned short shift;
  unsigned short HexAND;
  unsigned short shiftedHexAND;


  printf ("Original Hex: 0x%X\n", originalHEX);
  bitAND = 0xF000;
  shift = 12;
  HexAND = (originalHEX & bitAND);
  shiftedHexAND = HexAND >> shift;
  printf ("0x%X & 0x%X\n", originalHEX, bitAND);
  printf ("Hex: 0x%X\n", HexAND);
  printf ("Dec: %d\n", HexAND);
  printf ("Right Shift >> %d\n", shift);
  printf ("Hex: 0x%X\n", shiftedHexAND);
  printf ("Dec: %d\n", shiftedHexAND);
  printf ("---------------\n");


  printf ("Original Hex: 0x%X\n", originalHEX);
  bitAND = 0x0F00;
  shift = 8;
  HexAND = (originalHEX & bitAND);
  shiftedHexAND = HexAND >> shift;
  printf ("0x%X & 0x%X\n", originalHEX, bitAND);
  printf ("Hex: 0x%X\n", HexAND);
  printf ("Dec: %d\n", HexAND);
  printf ("Right Shift >> %d\n", shift);
  printf ("Hex: 0x%X\n", shiftedHexAND);
  printf ("Dec: %d\n", shiftedHexAND);
  printf ("---------------\n");

  printf ("Original Hex: 0x%X\n", originalHEX);
  bitAND = 0x00F0;
  shift = 4;
  HexAND = (originalHEX & bitAND);
  shiftedHexAND = HexAND >> shift;
  printf ("0x%X & 0x%X\n", originalHEX, bitAND);
  printf ("Hex: 0x%X\n", HexAND);
  printf ("Dec: %d\n", HexAND);
  printf ("Right Shift >> %d\n", shift);
  printf ("Hex: 0x%X\n", shiftedHexAND);
  printf ("Dec: %d\n", shiftedHexAND);
  printf ("---------------\n");

  printf ("Original Hex: 0x%X\n", originalHEX);
  bitAND = 0x000F;
  shift = 0;
  HexAND = (originalHEX & bitAND);
  shiftedHexAND = HexAND >> shift;
  printf ("0x%X & 0x%X\n", originalHEX, bitAND);
  printf ("Hex: 0x%X\n", HexAND);
  printf ("Dec: %d\n", HexAND);
  printf ("Right Shift >> %d\n", shift);
  printf ("Hex: 0x%X\n", shiftedHexAND);
  printf ("Dec: %d\n", shiftedHexAND);
  printf ("---------------\n");

  printf ("Hex: 0x%X\n", 0xFF);
  printf ("Dec: %d\n", 0XFF);
  printf ("---------------\n");

  unsigned short little = 32767;  // overflow for 32768, 2^16-1 = 65535
  little += little;
  printf ("Hex: 0x%X\n", little);
  printf ("Dec: %d\n", little);
  printf ("---------------\n");

  unsigned char littleCharX = 128;  // overflow for 128, 2^8-1 = 255
  unsigned char littleCharY = 128;

  if (littleCharX + littleCharY > 0xFF)
    printf ("Overflow\n");
  else
    printf ("Not Overflow\n");

  if (littleCharY > 255 - littleCharY)
    printf ("Overflow\n");
  else
    printf ("Not Overflow\n");

  littleCharX += littleCharY;
  printf ("Hex: 0x%X\n", littleCharX);
  printf ("Dec: %d\n", littleCharX);
  printf ("---------------\n");

  // BCD
  originalHEX = 0xA2;
  printf ("Original Hex: 0x%X\n", originalHEX);
  printf ("Original Hex: %d\n", originalHEX);
  // unsigned char hundreds = (originalHEX % 1000) / 100;
  // unsigned char tens = (originalHEX % 100) / 10;
  // unsigned char ones = (originalHEX % 10);
  unsigned char hundreds = (originalHEX / 100) % 10;
  unsigned char tens = (originalHEX / 10) % 10;
  unsigned char ones = (originalHEX) % 10;
  // unsigned char hundreds = originalHEX / 100;
  // unsigned char tens = (originalHEX / 10) % 10;
  // unsigned char ones = (originalHEX % 100) % 10;
  printf ("**hundreds**\n");
  printf ("Hex: 0x%X\n", hundreds);
  printf ("Dec: %d\n", hundreds);
  printf ("**tens**\n");
  printf ("Hex: 0x%X\n", tens);
  printf ("Dec: %d\n", tens);
  printf ("**ones**\n");
  printf ("Hex: 0x%X\n", ones);
  printf ("Dec: %d\n", ones);
  printf ("---------------\n");


  printf ("0x%X\n", 0b10000000 >> 7);
  printf ("0x%X\n", 0b01111111 >> 7);
  printf ("0x%X\n", 0x00 & 0x7);
  printf ("0x%X\n", 0xFF >> 1);
  printf ("%d\n", 0xFF << 1);
  printf ("---------------\n");

  printf ("**Random Number**\n"); // rand() % (MAX + 1 - MIN) + MIN
  unsigned char num = (rand() % (0xFF - 0x00 + 1)) + 0x00;
  printf ("Hex: 0x%X\n", num);
  printf ("Dec: %d\n", num);
  printf ("---------------\n");

  printf ("SCREEN_SIZE = %d\n", SCREEN_SIZE);
  printf ("SCREEN_WIDTH = %d\n", SCREEN_WIDTH);
  printf ("SCREEN_HEIGHT = %d\n", SCREEN_HEIGHT);



  return 0;


  // TEMPLATE //
  /* Set up render system and register input callbacks  */

  /* Initialize the Chip8 system and load the game into the memory */
  // if(argc < 2)
  // {
  //   printf("No File given\n");
	// 	return 1;
  // }
  // myChip8.loadApplication(argv[1]); // initialization included

  /* Emulation loop */
  // for(;;)
  // {
  //   /* Emulate one cycle */
  //   myChip8.emulateCycle();
  //
  //   /* If the draw flag is set, update the screen */
  //   if(myChip8.drawFlag)
  //     drawGraphics();
  //
  //   /* Store key press state (Press and Release) */
  //   myChip8.setKeys();
  // }

}
