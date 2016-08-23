#pragma once
#include "geometry.hpp"
class Plane :
		public Geometry
{
public:

	Plane(Shader* shader, glm::vec3 position, GLuint reflectionTexId, GLuint refractionTexId);

	~Plane(void)
	{
	}

	static GLfloat vertices[];
	static const int SIZE = 1;
    float WAVE_SPEED = 0.03f;

    void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, float deltaTime);

private:
    GLuint reflectionTexId;
    GLuint refractionTexId;
    GLuint normalMapTexId;
	GLuint dudvMapTexId;
    float moveFactor = 0.0f;
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

Plane::Plane(Shader* shader, glm::vec3 position, GLuint reflectionTexId, GLuint refractionTexId) : Geometry(shader, position, vertices, sizeof(vertices), NULL, GL_TEXTURE_2D)
{
	this->reflectionTexId = reflectionTexId;
	this->refractionTexId = refractionTexId;

	this->dudvMapTexId = loadTexture("../textures/waterDUDV.png");
}

void Plane::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, float deltaTime) {
    _shader->enableShader();
    _shader->setUniformMatrix4fv("projection", projectionMatrix);
    _shader->setUniformMatrix4fv("view", viewMatrix);
    _shader->setUniformMatrix4fv("model", _modelMatrix);
    // tell our shader that refraction texture is in unit 1
    _shader->setUniform1i("refractionTexture", 1);
    _shader->setUniform1i("dudvMap", 2);

    moveFactor += WAVE_SPEED * deltaTime;
    // Make it loop
    moveFactor = fmod(moveFactor, 1.0f);
    _shader->setUniform1f("moveFactor", moveFactor);


    /* Tell GL we want to work on our VAO */
    glBindVertexArray(this->_VAO);

    // special texture treatment for water rendering
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->reflectionTexId);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->refractionTexId);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->dudvMapTexId);

    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, this->normalMapTexId);

    glDrawArrays(this->drawMode, 0, this->_triangleCount);
    glBindVertexArray(0);
}