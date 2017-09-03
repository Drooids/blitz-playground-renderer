#include "Game.h"

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

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

			/*
				GLEW obtains information on the supported extensions from the graphics driver. 
				Experimental or pre-release drivers, however, might not report every available extension 
				through the standard mechanism, in which case GLEW will report it unsupported. 
				To circumvent this situation, the glewExperimental global switch can be turned on by 
				setting it to GL_TRUE before calling glewInit(), which ensures that all extensions with valid entry points will be exposed.
			*/

			glewExperimental = GL_TRUE;

			glewInit();

			if(m_mainContext != 0) {
				std::cout << "Successful: Init (...)\n";

				// Shaders

				const char *vssrc =
					"#version 330 core\n"
					"layout (location = 0) in vec3 aPos;\n"
					"void main()\n"
					"{\n"
					"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
					"}\0";

				const char *fssrc =
					"#version 330 core\n"
					"out vec4 FragColor;\n"
					"void main()\n"
					"{\n"
					"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
					"}\n\0";

				// vs

				int vs = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vs, 1, &vssrc, NULL);
				glCompileShader(vs);

				int success;
				char infoLog[512];

				glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(vs, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}

				// fs

				int fs = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fs, 1, &fssrc, NULL);
				glCompileShader(fs);

				glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
				if (!success) {
					glGetShaderInfoLog(fs, 512, NULL, infoLog);
					 std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				}

				shaderProgram = glCreateProgram();
				glAttachShader(shaderProgram, vs);
				glAttachShader(shaderProgram, fs);
				glLinkProgram(shaderProgram);

				glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				}

				// glDeleteShader(vs);
				// glDeleteShader(fs);

				// User/Application data

				float vertices[] = {
					-0.5f, -0.5f, 0.0f, // left  
					0.5f, -0.5f, 0.0f, // right 
					0.0f,  0.5f, 0.0f  // top   
				};

				unsigned int VBO;

				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);

				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glBindVertexArray(0);

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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

void Game::render()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
