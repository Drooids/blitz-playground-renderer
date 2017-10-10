#include "Game.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

Game* Game::s_pInstance = 0;

Game* Game::Instance() {
	if (s_pInstance == 0) {
		s_pInstance = new Game();
		return s_pInstance;
	}

	return s_pInstance;
};

unsigned texture1, texture2;

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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

				glEnable(GL_DEPTH_TEST);

				m_shader = new Shader("graphics/shaders/texture.vs", "graphics/shaders/texture.fs");

				float vertices[] = {
					-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
					0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
					0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
					0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
					-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
					-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

					-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
					0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
					0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
					0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
					-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
					-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

					-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
					-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
					-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
					-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
					-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
					-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

					0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
					0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
					0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
					0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
					0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
					0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

					-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
					0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
					0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
					0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
					-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
					-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

					-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
					0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
					0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
					0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
					-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
					-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
				};
		
				unsigned int VBO;
				glGenVertexArrays(1, VAO);
				glGenBuffers(1, &VBO);

				glBindVertexArray(VAO[0]);

				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

				// position attribute
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);

				// texture coord attribute
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);

				glGenTextures(1, &texture1);
				// glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture1);

				glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

				// Flip
				stbi_set_flip_vertically_on_load(true);

				int w, h, nc;

				unsigned char *data = stbi_load("assets/mario.jpg", &w, &h, &nc, 0);
				if (data)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else {
					std::cout << "Failed...";
				}

				// ---

				glGenTextures(1, &texture2);
				// glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, texture2);

				glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				unsigned char *data1 = stbi_load("assets/moon.jpg", &w, &h, &nc, 0);
				if (data1)
				{
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
					glGenerateMipmap(GL_TEXTURE_2D);
				}
				else {
					std::cout << "Failed...";
				}

				stbi_image_free(data1);

				m_shader->use();
				m_shader->setInt("texture1", 0);
				m_shader->setInt("texture2", 1);

				// Unbind
				glBindVertexArray(0);

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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Wireframe
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	m_shader->use();

	// Camera

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.1f, 100.0f);
	m_shader->setMat4("projection", projection);

	camera.onInput(true, false, true); // drag, scroll, keypress

	glm::mat4 view;
	float radius = 10.0f;

	float camX = sin(SDL_GetTicks() / 1000.0f) * radius;
	float camZ = cos(SDL_GetTicks() / 1000.0f) * radius;
	// view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	
	view = camera.GetViewMatrix();
	m_shader->setMat4("view", view);

	// std::cout << "camX: " << camX << "camZ:" << camZ << endl;

	// Enable VAO to set axes data
	glBindVertexArray(VAO[0]);

	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_shader->setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// Draw axes
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw triangles using indicies
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Use the renderer of the state machine...
	// m_pGameStateMachine->render();

	SDL_GL_SwapWindow(m_pWindow);
}

void Game::update()
{
	// m_pGameStateMachine->update();
}

void Game::handleEvents() {

	TheInputHandler::Instance()->update();

	if (TheInputHandler::Instance()->onKeyDown(SDL_SCANCODE_RETURN)) {
		// m_pGameStateMachine->changeState(new PlayState());
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
