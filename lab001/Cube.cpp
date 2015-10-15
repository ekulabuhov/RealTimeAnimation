#include "Cube.hpp"

Cube::Cube()
{
	this->_shader = NULL;
	this->_modelMatrix = glm::mat4();

	/* Create our list of vertices */
	GLfloat vertices[] =
	{
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,

		0.5f, 0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
	};

	/* Create our list of vertex indices */
	GLuint indices[] =
	{
		/* Front Face */
		0, 3, 1,
		1, 3, 2,

		/* Right Face  */
		4, 0, 5,
		1, 5, 0,

		/* Back Face */
		7,4,6,
		4,5,6,

		/* Left Face */
		7,2,3,
		6,2,7,

		/* Top Face */
		7,3,4,
		4,3,0,

		/* Bottom Face */
		6,5,2,
		1,2,5
	};

	/* Get GL to allocate space for our array buffers */
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	glGenBuffers(1, &this->_EBO);

	/* Tell GL we're now working on our Vertex Array Object */
	glBindVertexArray(this->_VAO);
	
	/* Give GL our vertices */
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Give GL our indices */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	/* Tell GL we don't need to work on our buffers any more */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Cube::draw()
{
	/* If this cube has a shader, use it */
	if (this->_shader != NULL)
	{
		this->_shader->enableShader();
		this->_shader->setUniformMatrix4fv("modelMat", this->_modelMatrix);
	}

	/* Tell GL we want to work on our VAO */
	glBindVertexArray(this->_VAO);
	glDrawElements(GL_TRIANGLES, 6 * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Cube::setShader(Shader* s)
{
	this->_shader = s;
}

void Cube::setShader(std::string shader)
{
	Shader* s = ShaderManager::getShader(shader);
	if (s != NULL)
	{
		this->setShader(s);
	}
}

Shader* Cube::getShader()
{
	return this->_shader;
}

void Cube::rotate(glm::vec3 rotation)
{
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}