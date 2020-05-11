#pragma once
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


#include <GL/glew.h>
//#include "stb_image.h"

class Texture
{
public:
	Texture() = default;
	Texture(char* fileloc);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint textureID;
	int width, height, bitDepth;
	char* filelocation;
};

