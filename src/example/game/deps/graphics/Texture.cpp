#include "Texture.h"

#include <glew.h>

#include <stb_image.h>

Texture::Texture(const char* filePath)
{
	data = stb_load(filePath, &width, &height, &nrChannels, 0);

	unsigned int txture;
	glGenTextures(1, &textureIDs);

	glBindtextglBindTexture(GL_TEXTURE_2D, textureIDs);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}