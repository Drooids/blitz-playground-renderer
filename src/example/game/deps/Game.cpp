#include "Game.h"

Game* Game::s_pInstance = 0;

Game* Game::Instance() {
	if (s_pInstance == 0) {
		s_pInstance = new Game();
		return s_pInstance;
	}

	return s_pInstance;
};

bool Game::init(const char* title, int xpos, int ypos, int width,
	int height, int flags)
{

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {

		// Window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

		// Joystick Input handler
		TheInputHandler::Instance()->initializeJoysticks();

		if(m_pWindow != 0) {

			// Renderer
			// m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			m_mainContext = SDL_GL_CreateContext(m_pWindow);

			SetOpenGLAttributes();

			// This makes our buffer swap syncronized with the monitor's vertical refresh
			SDL_GL_SetSwapInterval(1);

			// Init GLEW
			// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
			#ifndef __APPLE__
			glewExperimental = GL_TRUE;
			glewInit();
			#endif

			if(m_mainContext != 0) {
				std::cout << "Successful: Init\n";

				// SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);

				// TheGameObjectFactory::Instance()->registerType("MenuButton", new MenuButtonCreator());
				// TheGameObjectFactory::Instance()->registerType("Player", new PlayerCreator());
				// TheGameObjectFactory::Instance()->registerType("Enemy", new EnemyCreator());
				// TheGameObjectFactory::Instance()->registerType("AnimatedGraphic", new AnimatedGraphicCreator());

				// m_pGameStateMachine = new GameStateMachine();
				// m_pGameStateMachine->changeState(new MainMenuState());

			} else
				return false;

		} else
			return false;

	} else
		return false;

	m_bRunning = true;

	return true;
}

bool Game::SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

void Game::render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Use the renderer of the state machine...
	// m_pGameStateMachine->render();

	SDL_GL_SwapWindow(m_pWindow);

	// SDL_RenderPresent(m_pRenderer);
}

void Game::update()
{
 	// m_pGameStateMachine->update();
}

void Game::handleEvents()
{
	TheInputHandler::Instance()->update();

	if (TheInputHandler::Instance()->onKeyDown(SDL_SCANCODE_RETURN)) {
		m_pGameStateMachine->changeState(new PlayState());
	}
}

bool Game::running()
{
	return m_bRunning;
}

void Game::clean()
{
	TheInputHandler::Instance()->clean();

	// Delete our OpengL context
	SDL_GL_DeleteContext(m_mainContext);

	// Destroy our window
	SDL_DestroyWindow(m_pWindow);

	SDL_Quit();
}

void Game::PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
