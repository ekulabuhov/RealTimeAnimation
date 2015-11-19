#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

#include <GL\glew.h>
#include <glm\mat4x4.hpp>
#include <glm\vec3.hpp>

class Shader
{
public:
	Shader();

	void enableShader();
	void disableShader();

	void setUniformVector4fv(std::string uniformName, glm::vec4 matrix);
	void setUniformMatrix4fv(std::string uniformName, glm::mat4x4 matrix);
	void setUniformVector3f(std::string uniformName, glm::vec3 v);
	void setUniform1f(std::string uniformName, GLfloat v);

	bool loadShadersFromFiles(std::string vertShaderPath, std::string fragShaderPath);

	GLuint _shaderProgramID;

protected:
	std::string _readShaderFile(std::string path);
	bool _compileShader(std::string source, GLuint shaderType, GLuint shaderID);
	bool _linkShaders(GLuint shaderProgram, GLuint vertShader, GLuint fragShader);

	GLuint _getUniformLocation(std::string uniform);

	GLuint _vertShaderID;
	GLuint _fragShaderID;
	

	std::map<std::string, GLuint> _shaderVariableLocations;
};

#endif