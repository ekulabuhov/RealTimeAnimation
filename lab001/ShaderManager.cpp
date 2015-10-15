#include "ShaderManager.hpp"

/* Initialise static member variables */
std::map<std::string, Shader*> ShaderManager::_shaders;
std::string ShaderManager::_shaderFolderPath = SHADER_MANAGER_DEFAULT_SHADER_FOLDER_PATH;
std::string ShaderManager::_vertShaderExt = SHADER_MANAGER_DEFAULT_VERT_SHADER_EXT;
std::string ShaderManager::_fragShaderExt = SHADER_MANAGER_DEFAULT_FRAG_SHADER_EXT;

bool ShaderManager::loadShader(std::string shader)
{
	std::stringstream vertShaderSS;
	vertShaderSS << _shaderFolderPath;
	vertShaderSS << shader;
	vertShaderSS << _vertShaderExt;

	std::stringstream fragShaderSS;
	fragShaderSS << _shaderFolderPath;
	fragShaderSS << shader;
	fragShaderSS << _fragShaderExt;

	Shader* s = new Shader();
	bool loaded = s->loadShadersFromFiles(vertShaderSS.str(), fragShaderSS.str());

	if (!loaded)
		return false;

	_shaders[shader] = s;

	return true;
}

bool ShaderManager::enableShader(std::string shader)
{
	/* Make sure we have such a shader first */
	if (_shaders.find(shader) != _shaders.end())
	{
		_shaders[shader]->enableShader();
	}

	return false;
}