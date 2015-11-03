#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.hpp"
#include "Entity.hpp"
#include "CubeMap.hpp"
#include "Cube.hpp"

GLFWwindow* window;
Cube* cube1;
bool keys[1024];
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
int xRot, yRot, zRot = 0;

bool initWindow()
{
	if (!glfwInit())
	{
		std::cout << "ERROR: Could not initialise GLFW...";
		std::cin;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "Lab 001", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "ERROR: Could not create winodw...";
		std::cin;
		return false;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "ERROR: Problem initialising GLEW: " << glewGetErrorString(err);
		std::cin;
		return false;
	}

	glViewport(0, 0, 800, 800);

	return true;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

int main()
{
	/* Create GL Window */
	if (!initWindow())
		return -1;

	/* Initialise vertex buffers for cube */
	CubeMap cubeMap;
	cubeMap.loadCubeMap("../textures/cubemaps/Yokohama/");

	/* Load shaders needed */
	
	Shader lightingShader = *ShaderManager::loadShader("lighting");
	Shader lampShader = *ShaderManager::loadShader("lamp");
	Shader simpleReflectShader = *ShaderManager::loadShader("simpleReflect");

	//Entity dragonModel;
	//dragonModel.loadFromFile("../models/monkey_MODEL.dae");
	//dragonModel.setShader(&lightingShader);

	glm::vec3 cube1Position(0.0f, 0.0f, 0.0F);
	cube1 = new Cube(&lightingShader, cube1Position);

	glm::vec3 lampPos(0.0f, 0.0f, 6.0f);
	//Cube lamp(&lampShader, lampPos);
	//lamp.scale(glm::vec3(0.2f));

	glm::mat4 projectionMatrix = glm::perspective(
		45.0f,
		1.0f,
		0.1f,
		1000.0f
	);

	glm::vec3 cameraPosition(0.0f, 0.0f, 5.0f);

	glm::mat4 viewMatrix = glm::lookAt(
		cameraPosition,
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f)
	);
  
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		/* Rendering Code */
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		do_movement();

		simpleReflectShader.enableShader();
		simpleReflectShader.setUniformMatrix4fv("projectionMat", projectionMatrix);
		simpleReflectShader.setUniformMatrix4fv("viewMat", viewMatrix);
		simpleReflectShader.setUniformVector4fv("camPos", glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
		//dragonModel.rotate(glm::vec3(0.0f, 0.01f, 0.0f));
		//dragonModel.draw();

		//lampPos = glm::vec3(sin(glfwGetTime()) * 2.0f, sin(glfwGetTime() / 2.0f) * 1.0f, 2.0f);

		lightingShader.enableShader();
		lightingShader.setUniformMatrix4fv("projection", projectionMatrix);
		lightingShader.setUniformMatrix4fv("view", viewMatrix);
		lightingShader.setUniformVector3f("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		lightingShader.setUniformVector3f("lightColor", glm::vec3(1.0f, 0.5f, 1.0f));
		lightingShader.setUniformVector3f("lightPos", lampPos);
		lightingShader.setUniformVector3f("viewPos", cameraPosition);

		//cube1->rotate(glm::vec3(0.01f, 0.00f, 0.00f));
		cube1->draw();

		//lampShader.enableShader();
		//lampShader.setUniformMatrix4fv("projection", projectionMatrix);
		//lampShader.setUniformMatrix4fv("view", viewMatrix);
		////lamp.setPosition(lampPos);
		////lamp.scale(glm::vec3(0.2f));
		//lamp.draw();
		
	    glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		//CubeMap::drawSkyBox();
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);

		glfwSwapBuffers(window);
		glfwPollEvents();
		int xRotAngle = (int)std::abs(((xRot % 630 + 630) / 1.75)) % 360;
		int yRotAngle = (int)std::abs(((yRot % 630 + 630) / 1.75)) % 360;
		int zRotAngle = (int)std::abs(((zRot % 630 + 630) / 1.75)) % 360;
		printf("x: %d, y: %d, z: %d\n", xRotAngle, yRotAngle, zRotAngle);
	}

	glfwTerminate();
	return 0;
}

// Moves/alters the object positions based on user input
void do_movement()
{
	if (keys[GLFW_KEY_SPACE]) {
		cube1->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	if (keys[GLFW_KEY_W]) {
		yRot--;
		cube1->rotate(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_S]) {
		yRot++;
		cube1->rotate(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_D]) {
		xRot--;
		cube1->rotate(glm::vec3(0.0f, 0.01f, 0.0f));
	}
	if (keys[GLFW_KEY_A]) {
		xRot++;
		cube1->rotate(glm::vec3(0.0f, -0.01f, 0.0f));
	}
	if (keys[GLFW_KEY_Q]) {
		zRot++;
		cube1->rotate(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	if (keys[GLFW_KEY_E]) {
		zRot--;
		cube1->rotate(glm::vec3(0.0f, 0.0f, -0.01f));
	}
}