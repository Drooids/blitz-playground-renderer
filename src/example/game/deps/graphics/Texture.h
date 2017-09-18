#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
public:

	int width,
		height,
		nrChannels;

	unsigned char *data;

	unsigned int textureIDs[];

	Texture(const char* filePath) {}

private:

};

#endif
