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
	Texture(std::string fileLoc);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;	
	int width, height, bitDepth;

	std::string fileLocation;
};