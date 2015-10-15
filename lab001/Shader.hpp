#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL\glew.h>

class Shader
{
public:
	Shader();

	void enableShader();
	void disableShader();

	bool loadShadersFromFiles(std::string vertShaderPath, std::string fragShaderPath);

protected:
	std::string _readShaderFile(std::string path);
	bool _compileShader(std::string source, GLuint shaderType, GLuint shaderID);
	bool _linkShaders(GLuint shaderProgram, GLuint vertShader, GLuint fragShader);

	GLuint _vertShaderID;
	GLuint _fragShaderID;
	GLuint _shaderProgramID;
};

#endif