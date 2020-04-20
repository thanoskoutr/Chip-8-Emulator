#include <iostream>
#include "chip8.h"
#include <SDL2/SDL.h>
#include "cleanup.h"
#include <unordered_map>
using namespace std;

// Screen attributes
const int Screen_Width  = 640;
const int Screen_Height = 480;

Chip8 Chip8App;

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
 void logSDLError(std::ostream &os, const std::string &msg)
 {
	 os << msg << " error: " << SDL_GetError() << std::endl;
 }

 void display()
 {
	 Chip8App.emulateCycle();
	 if(Chip8App.drawFlag)
	 {
		 // Clear framebuffer
		 // Update texture
		 // Processed frame
		 myChip8.drawFlag = false;
	 }
 }


int main (int argc, char **argv)
{
	/* Initialize the Chip8 system and load the game into the memory */
	// if(argc < 2)
	// {
	//   printf("No File given\n");
	// 	return 1;
	// }

	if (Chip8App.loadApplication(argv[1]) != 0) // initialization included
	{
		std::cout << "Failed to Load Application" << std::endl;
		return 1;
	}


	// First we need to start up SDL, and make sure it went ok
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}
	// Now create a window with title "Hello World" at 100, 100 on the screen with w:640 h:480 and show it
	// Setup our window and renderer
	SDL_Window *window = SDL_CreateWindow(argv[1], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Screen_Width, Screen_Height, SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		logSDLError(std::cout, "CreateWindow");
		return 1;
	}

	// Create a renderer that will draw to the window, -1 specifies that we want to load whichever
	// video driver supports the flags we're passing
	// Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
	// 				SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
	// 				synchronized with the monitor's refresh rate
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr)
	{
		logSDLError(std::cout, "CreateRenderer");
		cleanup(window);
		SDL_Quit();
		return 1;
	}

	// To use a hardware accelerated texture for rendering we can create one
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (texture == nullptr)
	{
		cleanup(texture, renderer, window);
		logSDLError(std::cout, "CreateTexture");
		SDL_Quit();
		return 1;
	}

	// Create a mapping of SDL keyboard symbols to CHIP-8 keypad codes
	std::unordered_map<int,int> keymap
	{
		{SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
		{SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
		{SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
		{SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
		{SDLK_ESCAPE, -1}
	};


	//Our event structure
	SDL_Event e;
	// For tracking if we want to quit
	bool quit = false;
	while (!quit)
	{
		// Read any events that occured, for now we'll just quit if any event occurs
		while (SDL_PollEvent(&e))
		{
			//If user closes the window
			if (e.type == SDL_QUIT)
				quit = true;

			//If user presses any key
			if (e.type == SDL_KEYDOWN)
				quit = true;

			//If user clicks the mouse
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}

		// First clear the renderer
		SDL_RenderClear(renderer);
		// Draw the texture
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		// Update the screen
		SDL_RenderPresent(renderer);
	}

	// Clean up our objects and quit
	cleanup(renderer, window);
	SDL_Quit();

	return 0;
}
