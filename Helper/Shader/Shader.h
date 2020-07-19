#pragma once
#include <fstream>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <string.h>
#include <unordered_map>
#include <string>

#include "./../commonValues.cpp"

class Shader
{
public:
	Shader();
	void CreateFromFiles(
		std::string vShader, 
		std::string fShader, 
		std::string gShader = "", 
		std::string tesShader = "", 
		std::string tcsShader = ""
	);
	
	void UseShader() { glUseProgram(this->shader); }

	~Shader();

private:
	GLuint shader;
	std::string readShaderFromFile(const char* shaderPath);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void CompileShaders(
		std::string vShaderCode, 
		std::string fShaderCode, 
		std::string gShaderCode = "", 
		std::string tesShaderCode = "", 
		std::string tcsShaderCode = ""
	);

public:
	GLuint GetShaderProgramID() { return this->shader; }

};

