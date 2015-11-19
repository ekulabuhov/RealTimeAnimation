#pragma once
#include "geometry.hpp"
class Plane :
	public Geometry
{
public:

	Plane(Shader* shader, glm::vec3 position, char* texturePath) : Geometry(shader, position, vertices, 48 * sizeof(GLfloat), texturePath)
	{
	}

	~Plane(void)
	{
	}

	static GLfloat vertices[];
};

GLfloat Plane::vertices[] = {
    // Positions          // Normals         // Texture Coords
    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
    -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f,
    - 25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f
};