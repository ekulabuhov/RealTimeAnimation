#ifndef SHADER_HPP
#define SHADER_HPP

#include <iostream>
#include <fstream>
#include <sstream>

class Shader
{
public:
	void enableShader();
	bool loadShadersFromFiles(std::string vertShaderPath, std::string fragShaderPath);

protected:
	std::string _readShaderFile(std::string path);
};

#endif