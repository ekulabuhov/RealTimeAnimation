#pragma once

#include <SOIL.h>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include "Shader.hpp"


class CubeMap{
private:
	void create_cube_map (
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right,
	GLuint* tex_cube);

	GLuint _VBO;
	GLuint _VAO;
	GLuint _EBO;

	bool load_cube_map_side (GLuint texture, GLenum side_target, const char* file_name);

public:
	CubeMap();
	GLuint texID;

	void loadCubeMap(const char* mapFolder);
	void drawSkyBox(Shader shader);
	void use();
};