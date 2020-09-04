#include "Shader.h"

Shader::Shader()
{
	shader = 0;
}

void Shader::CreateFromFiles(
	std::string vShader, 
	std::string fShader, 
	std::string gShader, 
	std::string tesShader, 
	std::string tcsShader,
	std::vector<const char*>* transformFeedbackOutputs
) {

	std::string vShaderCode = readShaderFromFile(vShader.c_str());
	std::string fShaderCode = readShaderFromFile(fShader.c_str());

	std::string gShaderCode = "";
	std::string tesShaderCode = "";
	std::string tcsShaderCode = "";
	if (gShader != "") {
		gShaderCode = readShaderFromFile(gShader.c_str());
	}

	if (tesShader != "") {
		tesShaderCode = readShaderFromFile(tesShader.c_str());
	}

	if (tcsShader != "") {
		tcsShaderCode = readShaderFromFile(tcsShader.c_str());
	}

	CompileShaders(
		vShaderCode.c_str(), 
		fShaderCode.c_str(), 
		gShaderCode.c_str(),
		tesShaderCode.c_str(),
		tcsShaderCode.c_str()
	);
}

Shader::~Shader()
{

}

std::string Shader::readShaderFromFile(const char* shaderPath)
{
	std::string code;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);

	try {
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		code = shaderStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Shader File " << shaderPath << " cannot be read " << std::endl;
	}

	return code;
}


void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
	GLuint theShader = glCreateShader(shaderType);

	const GLchar* theCode[1];
	theCode[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);    
	if (!result) {
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling the " << shaderType << " shader " << eLog << std::endl;
		return;
	}

	glAttachShader(theProgram, theShader);
}


void Shader::CompileShaders(
	std::string vShaderCode, 
	std::string fShaderCode, 
	std::string gShaderCode, 
	std::string tesShaderCode, 
	std::string tcsShaderCode,
	std::vector<const char*>* transformFeedbackOutputs
) {
	shader = glCreateProgram(); 

	if (!shader) {
		std::cout << "Error creating shader program" << std::endl;
		return;
	}

	AddShader(shader, vShaderCode.c_str(), GL_VERTEX_SHADER);
	AddShader(shader, fShaderCode.c_str(), GL_FRAGMENT_SHADER);

	if (gShaderCode != "") {
		AddShader(shader, gShaderCode.c_str(), GL_GEOMETRY_SHADER);
	}

	if (tesShaderCode != "") {
		AddShader(shader, tesShaderCode.c_str(), GL_TESS_EVALUATION_SHADER);
	}

	if (tcsShaderCode != "") {
		AddShader(shader, tcsShaderCode.c_str(), GL_TESS_CONTROL_SHADER);
	}

	// Setting up transform feedback if transformFeedbackOutputs is not nullptr
	// Must be done before linking the program
	if (transformFeedbackOutputs) {
		glTransformFeedbackVaryings(
			shader, 
			transformFeedbackOutputs->size(), 
			transformFeedbackOutputs->data(),
			GL_SEPARATE_ATTRIBS 
		);
	}

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);  
	glGetProgramiv(shader, GL_LINK_STATUS, &result);    
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(shader);  
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);    
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: " << eLog << std::endl;
		return;
	} 
}

void Shader::RegisterUniform(std::string uniformName)
{
	uniforms[uniformName] = glGetUniformLocation(shader, uniformName.c_str());
}

GLuint Shader::GetUniformLocation(std::string uniformName)
{
	std::unordered_map<std::string, GLuint>::iterator pos = uniforms.find(uniformName);

	if (pos == uniforms.end()) {
		RegisterUniform(uniformName);
	}

	return uniforms[uniformName];
}

void Shader::SetUniform(std::string uniformName, float value)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniform1f(uniformLocation, value);
}

void Shader::SetUniform(std::string uniformName, int value)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniform1i(uniformLocation, value);
}

void Shader::SetUniform(std::string uniformName, bool value)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniform1i(uniformLocation, value);
}

void Shader::SetUniform(std::string uniformName, const glm::vec2& v)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniform2f(uniformLocation, v.x, v.y);
}

void Shader::SetUniform(std::string uniformName, const glm::vec3& v)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniform3f(uniformLocation, v.x, v.y, v.z);
}

void Shader::SetUniform(std::string uniformName, const glm::vec4& v)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniform4f(uniformLocation, v.x, v.y, v.z, v.w);
}

void Shader::SetUniform(std::string uniformName, const glm::mat3& v)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniformMatrix3fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(v));
}

void Shader::SetUniform(std::string uniformName, const glm::mat4& v)
{
	GLuint uniformLocation = GetUniformLocation(uniformName.c_str());
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(v));
}
