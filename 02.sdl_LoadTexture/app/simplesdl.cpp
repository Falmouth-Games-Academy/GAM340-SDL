#include <SDL.h>
#include <SDL_image.h>

#include <stdlib.h>
#include <stdio.h>

#include <windows.h>

#undef  main

#include <stdlib.h> //rand()

#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2_image.lib")

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int main(int argc, char *argv[]) 
{
	//Initialise IMG system, this allows us to load PNG files
	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags);

	//Initialise window & renderer
	window = NULL;
	renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//create a window that is 800x600 pixels
	window = SDL_CreateWindow("title", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	//load a bitmap using the IMG library
	SDL_Surface* srcImage = IMG_Load("resources/ball.png");

	if (srcImage == NULL)
	{
		SDL_Log("SDL_Surface: can't load image\n");
		goto quit;
	}

	SDL_Texture* ballTexture;
	
	//convert the
	ballTexture = SDL_CreateTextureFromSurface(renderer, srcImage);
	
	if (ballTexture == NULL)
	{
		SDL_Log("SDL_Texture: can't create texture\n");
		goto quit;
	}

	
	
	Uint32 lastElapsedTime=0, fpsTimer=0;
	
	Uint32 elaspedTime = 0;
	Uint32 fpsTime = 0;

	SDL_Event event;
	bool quitting = false;

	//The main application loop
	//This uses SDL_GetTicks() to record elapsed time and pause the application with Sleep to keep it running at 60fps rather than thousands of fps

	while (!quitting)
	{	
		fpsTime = SDL_GetTicks() - lastElapsedTime;
		lastElapsedTime = SDL_GetTicks();

		// SDL uses events to communicate with the application
		// In this instance, the app is just responding to SDL_QUIT messages which occur when the application is closed
		// to close the app down, either press the X icon on the window or get you program to generation an SDL_QUIT event internally
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				quitting = true;
			}
		}

		//draw the ball using SDL_RenderCopy
		//this requires a rectangle of the ball's position and size (for scalability)
		//to get the size of the texture, we can use SDL_QueryTexture
		SDL_Rect dest;

		dest.x = 0;
		dest.y = 0;

		Uint32 format;
		int access;
		SDL_QueryTexture(ballTexture, &format, &access, &dest.w, &dest.h);
			
		SDL_RenderCopy(renderer, ballTexture, NULL, &dest);

		//This uses the window title to display frame performance information
		{
			char temp[255];

			sprintf(temp, "Frame Time:%.3f  (%3.0f FPS)", (float)(fpsTime), 1000.0f/fpsTime);
	
			SDL_SetWindowTitle(window, temp);
		}
			
		//End of frame rendering
		SDL_RenderPresent(renderer);							
				
		//Sleep the app					
		if (elaspedTime < 16)
		{
			Sleep(16 - elaspedTime);					
		}							
	}
	
	quit:
		//On end of application, close down resources and exit
		SDL_DestroyWindow(window);
		SDL_Quit();

		window = NULL;
		renderer = NULL;

	return 0;
}