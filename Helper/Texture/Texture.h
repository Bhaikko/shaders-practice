#pragma once

#include <iostream>
#include <string>

#include <GL/glew.h>

// #include "./../stb_image.h"
#include "./../commonValues.cpp"

class Texture
{
public:
	Texture();
	~Texture();

	void LoadTexture(std::string fileLocation);
	void LoadCubMap(std::string files[]);
	void UseTexture(GLenum textureUnit);
	void UseCubeMap(GLenum textureUnit);
	void ClearTexture();

private:
	GLuint textureID;	
	int width, height, bitDepth;
};