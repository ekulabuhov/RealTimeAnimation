#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.hpp"
#include "Entity.hpp"
#include "CubeMap.hpp"

GLFWwindow* window;

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

int main()
{
	/* Create GL Window */
	if (!initWindow())
		return -1;

	/* Initialise vertex buffers for cube */
	CubeMap cubeMap;
	cubeMap.loadCubeMap("../textures/cubemaps/Yokohama/");

	/* Load shaders needed */
	

	Entity dragonModel;
	dragonModel.loadFromFile("../models/monkey_MODEL.dae");
	dragonModel.setShader(ShaderManager::loadShader("simpleReflect"));

	glm::mat4 projectionMatrix = glm::perspective(
		45.0f,
		1.0f,
		0.1f,
		1000.0f
	);

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f,0.0f,5.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f)
	);
  
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		/* Rendering Code */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dragonModel.getShader()->setUniformMatrix4fv("projectionMat", projectionMatrix);
		dragonModel.getShader()->setUniformMatrix4fv("viewMat", viewMatrix);
		dragonModel.getShader()->setUniformVector4fv("camPos", glm::vec4(0.0f, 0.0f, 5.0f, 1.0f));
		dragonModel.rotate(glm::vec3(0.0f, 0.01f, 0.0f));
		dragonModel.draw();
		
	    glDisable(GL_TEXTURE_GEN_S);
		glDisable(GL_TEXTURE_GEN_T);
		glDisable(GL_TEXTURE_GEN_R);
		//CubeMap::drawSkyBox();
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
		glEnable(GL_TEXTURE_GEN_R);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
