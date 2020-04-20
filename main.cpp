#include <iostream>
#include "chip8.h"
#include <SDL2/SDL.h>
using namespace std;

int main (int argc, char **argv)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Quit();
	return 0;

}
