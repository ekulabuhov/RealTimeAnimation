#ifndef CUBE_HPP
#define CUBE_HPP

#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Geometry.hpp"

class Cube: public Geometry
{
static GLfloat vertices[];
static GLuint indices[];
public:
	Cube(Shader* shader, glm::vec3 position);
};

#endif