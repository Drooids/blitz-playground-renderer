#include "deps/Game.h"
#include <iostream>
#include "main.h"

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char *argv[])
{
	// SDL_WINDOW_FULLSCREEN
	// SDL_WINDOW_FULLSCREEN_DESKTOP
	// SDL_WINDOW_BORDERLESS
	if(TheGame::Instance()->init("Blitz Engine", 100, 100, 800, 600, SDL_WINDOW_OPENGL)) {

		while(TheGame::Instance()->running()) {

			TheGame::Instance()->frameStart = SDL_GetTicks();

			TheGame::Instance()->handleEvents();
			// TheGame::Instance()->update();
			TheGame::Instance()->render();

			// How many ms did it take after handling, updating and rendering
			TheGame::Instance()->frameTime = SDL_GetTicks() - TheGame::Instance()->frameStart;

			/*
				First we get the time at the start of our loop and store it in frameStart. For this we
				use SDL_GetTicks which returns the amount of milliseconds since we called SDL_
				Init. We then run our game loop and store how long it took to run by subtracting
				the time our frame started from the current time. If it is less than the time we want a
				frame to take, we call SDL_Delay and make our loop wait for the amount of time we
				want it to, subtracting how long the loop already took to complete.
			*/
			if (TheGame::Instance()->frameTime < DELAY_TIME) {
				SDL_Delay((int)(DELAY_TIME -TheGame::Instance()->frameTime));
			}
		}

	} else {
		std::cout << SDL_GetError() << "\n";
	}

	TheGame::Instance()->clean();

	return 0;
}
