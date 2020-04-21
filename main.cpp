#include <iostream>
#include "chip8.h"
#include <SDL2/SDL.h>
#include "cleanup.h"
#include <unordered_map>
#include <chrono>
#include <thread>
// using namespace std;

// Screen attributes
const int Screen_Width  = 640;
const int Screen_Height = 480;

Chip8 chip8;

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
	 chip8.emulateCycle();
	 if(chip8.drawFlag)
	 {
		 // Clear framebuffer
		 // Update texture
		 // Processed frame
		 chip8.drawFlag = false;
	 }
 }


int main (int argc, char **argv)
{
	/* Initialize the Chip8 system and load the game into the memory */
	if(argc < 2)
	{
	  printf("No ROM given...\n");
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
	std::unordered_map<int,unsigned char> keymap
	{
		{SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
		{SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
		{SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
		{SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF},
		{SDLK_ESCAPE, -1}
	};


  // Temporary pixel buffer for updating texture
  uint32_t pixels[2048];

  // Load Application
  if (chip8.loadApplication(argv[1]) != 0) // initialization included
	{
		std::cout << "Failed to Load Application" << std::endl;
		return 1;
	}

	// Our event structure
	SDL_Event e;
	// For tracking if we want to quit
	bool quit = false;

  // Emulation loop
	while (!quit)
	{
    chip8.emulateCycle();

		// Read any SDL events that occured
		while (SDL_PollEvent(&e))
		{
			//If user closes the window
			if (e.type == SDL_QUIT)
				quit = true;

			// If user presses any key
			if (e.type == SDL_KEYDOWN)
      {
        // If user presses ESC we exit
        if (e.key.keysym.sym == SDLK_ESCAPE)
          quit = true;
        else
        {
          // find if valid is key pressed and set to 1
          auto pressed_key = keymap.find(e.key.keysym.sym);
          if (pressed_key != keymap.end())
          {
            printf ("Pressed Key: 0x%X\n", pressed_key->first);
            printf ("Mapped to: 0x%X\n", pressed_key->second);
            chip8.key[pressed_key->second] = 1;
          }

        }
      }

      // If user releases any key
      if (e.type == SDL_KEYUP)
      {
        // find if valid is key pressed and set to 0
        auto released_key = keymap.find(e.key.keysym.sym);
        if (released_key != keymap.end())
        {
          printf ("Released Key: 0x%X\n", released_key->first);
          printf ("Mapped to: 0x%X\n", released_key->second);
          chip8.key[released_key->second] = 0;
        }

      }
		}

    // If draw occurred, redraw SDL screen
    if(chip8.drawFlag)
 	 {
     chip8.drawFlag = false;

     // Store pixels in temporary buffer
     for (int i = 0; i < SCREEN_SIZE; ++i)
     {
       unsigned char pixel = chip8.gfx[i];
       pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
     }
     // Update SDL texture
     SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH* sizeof(Uint32));
     // First clear the renderer
     SDL_RenderClear(renderer);
     // Draw the texture
     SDL_RenderCopy(renderer, texture, NULL, NULL);
     // Update the screen
     SDL_RenderPresent(renderer);
 	 }

   // Sleep to slow down emulation speed
   // std::this_thread::sleep_for(std::chrono::microseconds(1200));
   SDL_Delay(100/60);

	}

	// Clean up our objects and quit
	cleanup(renderer, window);
	SDL_Quit();

	return 0;
}
