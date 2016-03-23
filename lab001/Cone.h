#pragma once
#include "Geometry.hpp"
class Cone :
	public Geometry
{
static GLfloat vertices[];
public:
	Cone(Shader* shader);
	Cone(Shader* shader, glm::vec3 position);
	~Cone();
};

