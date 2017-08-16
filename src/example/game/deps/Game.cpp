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

			if(m_mainContext != 0) {

				std::cout << "Successful: Init\n";

				const char *vertexShaderSource = "#version 330 core\n"
					"layout (location = 0) in vec3 aPos;\n"
					"void main()\n"
					"{\n"
					"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
					"}\0";

				const char *fragmentShaderSource = "#version 330 core\n"
					"out vec4 FragColor;\n"
					"void main()\n"
					"{\n"
					"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
					"}\n\0";

				// Init glew
				glewInit();

				// build and compile our shader program
				// ------------------------------------
				// vertex shader
				int vertexShader = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
				glCompileShader(vertexShader);

				// check for shader compile errors
				int success;
				char infoLog[512];
				glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
				}
				// fragment shader
				int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
				glCompileShader(fragmentShader);

				// check for shader compile errors
				glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
				}

				// link shaders
				shaderProgram = glCreateProgram();

				glAttachShader(shaderProgram, vertexShader);
				glAttachShader(shaderProgram, fragmentShader);
				glLinkProgram(shaderProgram);

				// check for linking errors
				glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
				if (!success) {
					glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
					std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
				}

				glDeleteShader(vertexShader);
				glDeleteShader(fragmentShader);

				// set up vertex data (and buffer(s)) and configure vertex attributes
				// ------------------------------------------------------------------
				float vertices[] = {
					-0.5f, -0.5f, 0.0f, // left  
					0.5f, -0.5f, 0.0f, // right 
					0.0f,  0.5f, 0.0f  // top   
				};

				unsigned int VBO;
				glGenVertexArrays(1, &VAO);
				glGenBuffers(1, &VBO);
				// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
				glBindVertexArray(VAO);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
				// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
				glBindVertexArray(0);

				// glBindVertexArray(0); // no need to unbind it every time 

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

	// render
	// ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// draw our first triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
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
