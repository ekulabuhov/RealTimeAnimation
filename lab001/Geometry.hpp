#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
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
	GLenum textureTarget;

	GLuint _VBO;
	GLuint _EBO;

	void Init(Shader* shader, glm::vec3 position,
			  GLfloat vertices[], int sizeOfVertices, GLuint texId = NULL, GLenum textureTarget = NULL);

protected:
	Shader* _shader;
	GLuint _VAO;
	GLuint _triangleCount;
	GLenum drawMode;
	GLuint loadTexture(GLchar* path);
public:
	// this overload is useful when you need to load a texture
	Geometry(Shader* shader, glm::vec3 position,
			 GLfloat vertices[], int sizeOfVertices, GLchar* texturePath);

	// this overload is useful when you have a texture loaded already (for cubemaps or render buffers)
	Geometry(Shader* shader, glm::vec3 position,
			 GLfloat vertices[], int sizeOfVertices, GLuint texId, GLenum textureTarget);

	// this overload is useful when you don't need a texture
	Geometry(Shader* shader, glm::vec3 position,
			 GLfloat vertices[], int sizeOfVertices);

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