#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <unordered_map>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	void UseShader() { glUseProgram(this->shader); }
	void RegisterUniform(std::string uniformName);
	GLuint GetUniformLocation(std::string uniformName);

private:
	std::unordered_map<std::string, GLuint> uniforms;

public:
	void SetUniform(std::string uniformName, float value);
	void SetUniform(std::string uniformName, int value);
	void SetUniform(std::string uniformName, bool value);
	void SetUniform(std::string uniformName, const glm::vec2& v);
	void SetUniform(std::string uniformName, const glm::vec3& v);
	void SetUniform(std::string uniformName, const glm::vec4& v);
	void SetUniform(std::string uniformName, const glm::mat3& v);
	void SetUniform(std::string uniformName, const glm::mat4& v);

};

