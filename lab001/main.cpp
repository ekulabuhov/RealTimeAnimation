#include <iostream>
#include <fstream>
#include <sstream>

#include <GL\glew.h>

#include <GLFW/glfw3.h>

GLuint VBO;
GLuint VAO;
GLuint EBO;

GLuint vShader;
GLuint fShader;
GLuint shaderProgram;

void initBuffers()
{
	GLfloat vertices[] =
	{
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	GLuint indices[] = 
	{
		0, 1, 3,
		1, 2, 3
	};

	GLuint newVAO;
	glGenVertexArrays(1, &newVAO);
	VAO = newVAO;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initShaders(const char* vShaderFilePath, const char* fShaderFilePath)
{
	std::ifstream fStream;
	fStream.open(vShaderFilePath);
	if (!fStream.is_open())
	{
		std::cout << "Could not open vertex shader file: " << vShaderFilePath << std::endl;
		return;
	}
	std::stringstream buffer;
	buffer << fStream.rdbuf();
	std::string vertShaderSource = buffer.str();
}

void drawCube()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

int main()
{
	GLFWwindow* window;

	if (!glfwInit())
	{
		std::cout << "ERROR: Could not initialise GLFW...";
		std::cin;
		return -1;
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
		return -1;
	}

	glfwMakeContextCurrent(window);
	
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "ERROR: Problem initialising GLEW: " << glewGetErrorString(err);
		std::cin;
		return -1;
	}


	glViewport(0, 0, 800, 800);

	initBuffers();

	initShaders("..\\shaders\\simple.vert", "");

	while (!glfwWindowShouldClose(window))
	{
		/* Rendering Code */
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		drawCube();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}