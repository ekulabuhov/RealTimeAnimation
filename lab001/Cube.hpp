#ifndef MODEL_HPP
#define MODEL_HPP

#include <GL\glew.h>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "ShaderManager.hpp"

class Cube
{
private:
	glm::vec3 _rotation;
	glm::vec3 _position;
	glm::mat4x4 _modelMatrix;

	GLuint _VBO;
	GLuint _VAO;
	GLuint _EBO;

	Shader* _shader;

public:
	Cube();

	void draw();

	void rotate(glm::vec3 r);
	void setRotation(glm::vec3 r);

	void setShader(Shader* s);
	void setShader(std::string shader);
	Shader* getShader();
};

#endif