#define STB_IMAGE_IMPLEMENTATION
#include "Texture.h"
#include "stb_image.h"

Texture::Texture(char* fileloc) : textureID{0}, width{0}, height{0}, bitDepth{0}, filelocation{fileloc} { }


void Texture::LoadTexture()
{
	// unsigned char=1 byte; below is same as array bytes to raw texture data 
	unsigned char* textData = stbi_load(filelocation, &width, &height, &bitDepth, 0);
	if (!textData)
	{
		printf("Failed to find: %s\n", filelocation);
		return;
	}

	glGenTextures(1, &textureID);				// textureID is the variable where the buffer location for the texture given by OpenGL; similar to VAO..
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // wrap on the x axis if we go over the border
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  // wrap on the y axis if we go over the border
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textData);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);  // unbind the buffer

	stbi_image_free(textData);	      // free the memory set to load the data

}

void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);				// GL_TEXTURE0 is identified as a TextureUnit; typicall number of TextureUnits in GPA are 32			
	glBindTexture(GL_TEXTURE_2D, textureID);	// the data loaded into buffer textureID will be binded to Texture Unit we specify
}												// GL_TEXTURE_2D is a GL enum that specifies it is a 2D texture; 
												// there is also 3d version which is used for volumertric effect such as clous

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	filelocation = nullptr;			// the class uses filelocation = ""; gives an error const char* can not be assigned to " " ????	
}

Texture::~Texture() { ClearTexture();}
