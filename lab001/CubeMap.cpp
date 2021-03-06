#include "CubeMap.hpp"

CubeMap::CubeMap()
{
	float points[] = {
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};

	glGenBuffers (1, &this->_VBO);
	glBindBuffer (GL_ARRAY_BUFFER, this->_VBO);
	glBufferData (GL_ARRAY_BUFFER, 3 * 36 * sizeof (float), &points, GL_STATIC_DRAW);


	glGenVertexArrays (1, &this->_VAO);
	glBindVertexArray (this->_VAO);
	glEnableVertexAttribArray (0);
	glBindBuffer (GL_ARRAY_BUFFER, this->_VBO);
	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, NULL);
	glBindVertexArray(NULL);
}

void CubeMap::use()
{
}


void CubeMap::loadCubeMap(const char* mapFolder){
	std::stringstream map[6];
	const char* format = ".png";

	map[0] << mapFolder << "negz" << format;
	map[1] << mapFolder << "posz" << format;
	map[2] << mapFolder << "posy" << format;
	map[3] << mapFolder << "negy" << format;
	map[4] << mapFolder << "negx" << format;
	map[5] << mapFolder << "posx" << format;

	std::cout << map[0].str().c_str() << std::endl;
	std::cout << map[1].str().c_str() << std::endl;
	std::cout << map[2].str().c_str() << std::endl;
	std::cout << map[3].str().c_str() << std::endl;
	std::cout << map[4].str().c_str() << std::endl;
	std::cout << map[5].str().c_str() << std::endl;

	this->create_cube_map(
		map[0].str().c_str(),
		map[1].str().c_str(),
		map[2].str().c_str(),
		map[3].str().c_str(),
		map[4].str().c_str(),
		map[5].str().c_str(),
		&texID
	);
}

void CubeMap::create_cube_map (
	const char* front,
	const char* back,
	const char* top,
	const char* bottom,
	const char* left,
	const char* right,
	GLuint* tex_cube
	) {
	// generate a cube-map texture to hold all the sides
	glActiveTexture (GL_TEXTURE0);
	glGenTextures (1, tex_cube);

	// load each image and copy into a side of the cube-map texture
	load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, front);
	load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, back);
	load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top);
	load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom);
	load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left);
	load_cube_map_side (*tex_cube, GL_TEXTURE_CUBE_MAP_POSITIVE_X, right);
	// format cube map texture
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

bool CubeMap::load_cube_map_side (
	GLuint texture, GLenum side_target, const char* file_name
	) {
	glBindTexture (GL_TEXTURE_CUBE_MAP, texture);
	int width, height;
	unsigned char* image;
	image = SOIL_load_image(file_name, &width, &height, 0, SOIL_LOAD_RGB);

	if(image != NULL){
		// copy image data into 'target' side of cube map
		glTexImage2D (
			side_target,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			image
			);
		return true;
	}else{
		std::cout << "Couldn't load texture: " << file_name << std::endl;
        return false;
	}
}

void CubeMap::drawSkyBox(Shader shader) {
	glDepthMask(GL_FALSE);
	shader.enableShader();
	glBindVertexArray(this->_VAO);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->texID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthMask(GL_TRUE);
}