#include "Geometry.hpp"

Geometry::Geometry(Shader* shader, glm::vec3 position,
				   GLfloat vertices[], int sizeOfVertices, GLuint indices[], int sizeOfIndices)
{
	this->_shader = shader;
	this->_modelMatrix = glm::mat4();
	this->_modelMatrix = glm::translate(this->_modelMatrix, position);

	/* Get GL to allocate space for our array buffers */
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	glGenBuffers(1, &this->_EBO);

	/* Tell GL we're now working on our Vertex Array Object */
	glBindVertexArray(this->_VAO);
	
	/* Give GL our vertices */
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	/* Give GL our indices */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	//glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	/* Tell GL we don't need to work on our buffers any more */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Geometry::draw()
{
	/* If this cube has a shader, use it */
	if (this->_shader != NULL)
	{
		this->_shader->enableShader();
		this->_shader->setUniformMatrix4fv("model", this->_modelMatrix);
	}

	/* Tell GL we want to work on our VAO */
	glBindVertexArray(this->_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
	glBindVertexArray(0);
}

void Geometry::setShader(Shader* s)
{
	this->_shader = s;
}

void Geometry::setShader(std::string shader)
{
	Shader* s = ShaderManager::getShader(shader);
	if (s != NULL)
	{
		this->setShader(s);
	}
}

Shader* Geometry::getShader()
{
	return this->_shader;
}

void Geometry::rotate(glm::vec3 rotation)
{
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	this->_modelMatrix = glm::rotate(this->_modelMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Geometry::move(glm::vec3 vector) 
{
	this->_modelMatrix = glm::translate(this->_modelMatrix, vector);
}

void Geometry::setPosition(glm::vec3 position) 
{
	glm::mat4 initialPos;
	this->_modelMatrix = glm::translate(initialPos, position);
}

void Geometry::scale(glm::vec3 scale)
{
	this->_modelMatrix = glm::scale(this->_modelMatrix, scale);
}