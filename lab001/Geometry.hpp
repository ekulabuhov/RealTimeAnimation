#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <GL\glew.h>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <SOIL.h>

#include "ShaderManager.hpp"

class Geometry
{
private:
	glm::vec3 _rotation;
	
	glm::mat4 _projectionMatrix;
	GLuint textureId;

	GLuint _VBO;
	GLuint _VAO;
	GLuint _EBO;

	Shader* _shader;

	GLuint _triangleCount;

public:
	Geometry(Shader* shader, glm::vec3 position,
		GLfloat vertices[], int sizeOfVertices, GLchar* texturePath = NULL);

	void draw();

	void rotate(glm::vec3 r);
	void setRotation(glm::vec3 r);
	void setRotationFromQuaternion(glm::quat quat);
	void scale(glm::vec3 scale);

	void setViewMatrix(glm::mat4 viewMatrix);

	void move(glm::vec3 r);
	void setPosition(glm::vec3 r);
	glm::vec3 getPosition();

	glm::vec3 localToWorld(glm::vec3 localPoint);

	glm::vec3 getUpVector();

	void setShader(Shader* s);
	void setShader(std::string shader);
	Shader* getShader();

	glm::mat4x4 _modelMatrix;

	glm::quat quaternion;
};

#endif