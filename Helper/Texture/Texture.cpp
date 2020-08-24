#include "Texture.h"

Texture::Texture()
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
}


void Texture::LoadTexture(std::string fileLocation, GLenum textureChannels)
{
	unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
	if (!texData) {
		std::cout << "Failed to find " << fileLocation << std::endl;
		return;
	}

	glGenTextures(1, &textureID);	
	glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		glTexImage2D(GL_TEXTURE_2D, 0, textureChannels, width, height, 0, textureChannels, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);	

	stbi_image_free(texData);	
}

void Texture::LoadCubMap(std::string files[])
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < 6; i++) {
		unsigned char* texData = stbi_load(files[i].c_str(), &width, &height, &bitDepth, 0);

		if (!texData) {
			std::cout << "Failed to load " << files[i] << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	
}

void Texture::UseTexture(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::UseCubeMap(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}

Texture::~Texture()
{
	ClearTexture();
}