#pragma once
#include "geometry.hpp"
class Plane :
		public Geometry
{
public:

	Plane(Shader* shader, glm::vec3 position, char* texturePath = NULL) : Geometry(shader, position, vertices, 48 * sizeof(GLfloat), texturePath)
	{
	}

	Plane(Shader* shader, glm::vec3 position, GLuint texId) : Geometry(shader, position, vertices, 48 * sizeof(GLfloat), texId, GL_TEXTURE_2D)
	{
	}

	~Plane(void)
	{
	}

	static GLfloat vertices[];
	static const int SIZE = 1;
};

GLfloat Plane::vertices[] = {
		// Positions          // Normals         // Texture Coords
		SIZE, 0.0f, SIZE, 0.0f, 1.0f, 0.0f, SIZE, 0.0f,
		-SIZE, 0.0f, -SIZE, 0.0f, 1.0f, 0.0f, 0.0f, SIZE,
		-SIZE, 0.0f, SIZE, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

		SIZE, 0.0f, SIZE, 0.0f, 1.0f, 0.0f, SIZE, 0.0f,
		SIZE, 0.0f, -SIZE, 0.0f, 1.0f, 0.0f, SIZE, SIZE,
		- SIZE, 0.0f, -SIZE, 0.0f, 1.0f, 0.0f, 0.0f, SIZE
};