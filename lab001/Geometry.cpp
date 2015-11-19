#include "Geometry.hpp"

GLuint loadTexture(GLchar* path);

Geometry::Geometry(Shader* shader, glm::vec3 position,
				   GLfloat vertices[], int sizeOfVertices, GLchar* texturePath)
{
	this->textureId = NULL;
	this->_shader = shader;
	this->_modelMatrix = glm::mat4();
	this->_modelMatrix = glm::translate(this->_modelMatrix, position);

	/* Get GL to allocate space for our array buffers */
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	//glGenBuffers(1, &this->_EBO);

	/* Tell GL we're now working on our Vertex Array Object */
	glBindVertexArray(this->_VAO);
	
	/* Give GL our vertices */
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	///* Give GL our indices */
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	//glEnableVertexAttribArray(0);

	int attribSize = texturePath ? 8 : 6;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribSize * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attribSize * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	if (texturePath) {
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, attribSize * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));

		this->textureId = loadTexture(texturePath);
	}

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
	if (this->textureId) {
		glBindTexture(GL_TEXTURE_2D, this->textureId);
	}
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

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
    // Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width, height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;

}