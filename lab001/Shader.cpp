#include "Shader.hpp"

bool Shader::loadShadersFromFiles(std::string vertShaderPath, std::string fragShaderFile)
{
	std::string vertShaderSource = this->_readShaderFile(vertShaderPath);
	std::string fragShaderSource = this->_readShaderFile(fragShaderFile);

	if (vertShaderSource == "" || fragShaderSource == "")
	{
		return false;
	}

	return true;
}

std::string Shader::_readShaderFile(std::string shaderPath)
{
	std::ifstream fStream;
	fStream.open(shaderPath);
	if (!fStream.is_open())
	{
		std::cerr << "Could not open shader file: " << shaderPath << std::endl;
		return "";
	}
	std::stringstream buffer;
	buffer << fStream.rdbuf();
	std::string shaderSource = buffer.str();

	return shaderSource;
}