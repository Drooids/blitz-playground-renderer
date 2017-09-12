#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

#include <glew.h>
#include <stb_image.h>

class Texture
{
public:

	int width,
		height,
		nrChannels;

	unsigned char *data;

	unsigned int textureID;

	Texture(const char* filePath)
	{
		data = stb_load(filePath, &width, &height, &nrChannels, 0);

		unsigned int txture;
		glGenTextures(1, &textureID);

		glBindtextglBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

private:

};

#endif
