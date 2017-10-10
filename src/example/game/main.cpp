#include "deps/Game.h"
#include <iostream>
#include "main.h"

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char *argv[])
{
	Game* game = TheGame::Instance();

	// SDL_WINDOW_FULLSCREEN
	// SDL_WINDOW_FULLSCREEN_DESKTOP
	// SDL_WINDOW_BORDERLESS
	if(game->init("Blitz Engine", 100, 100, 800, 600, SDL_WINDOW_OPENGL)) {

		while(game->running()) {

			// Not to get confused on the first look later on, I'll explain it to myself:
			// currentFrame takes the current time, dt is the difference between the current and last times
			// and finally we reset the time...
			game->currentFrame = SDL_GetTicks();
			game->dt = game->currentFrame - game->lastFrame;
			game->lastFrame = game->currentFrame;

			game->handleEvents();
			// game->update();
			game->render();

			// How many ms did it take after handling, updating and rendering
			game->frameTime = SDL_GetTicks() - game->frameStart;

			/*
				First we get the time at the start of our loop and store it in frameStart. For this we
				use SDL_GetTicks which returns the amount of milliseconds since we called SDL_
				Init. We then run our game loop and store how long it took to run by subtracting
				the time our frame started from the current time. If it is less than the time we want a
				frame to take, we call SDL_Delay and make our loop wait for the amount of time we
				want it to, subtracting how long the loop already took to complete.
			*/
			if (game->frameTime < DELAY_TIME) {
				SDL_Delay((int)(DELAY_TIME -game->frameTime));
			}
		}

	} else {
		std::cout << SDL_GetError() << "\n";
	}

	game->clean();

	return 0;
}
