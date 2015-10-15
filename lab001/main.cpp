#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.hpp"
#include "Cube.hpp"

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
	Cube cube1;
	Cube cube2;
	Cube cube3;

	/* Load shaders needed */
	cube1.setShader(ShaderManager::loadShader("cube1"));
	ShaderManager::loadShader("cube2");
	ShaderManager::loadShader("cube3");

	while (!glfwWindowShouldClose(window))
	{
		/* Rendering Code */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		ShaderManager::useShader("cube1");
		cube1.rotate(glm::vec3(0.0f, 0.0f, 1.0f));
		cube1.draw();

		//ShaderManager::useShader("cube2");
		//cube2.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
