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
