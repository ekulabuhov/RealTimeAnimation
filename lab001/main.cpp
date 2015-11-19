#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>

#include "ShaderManager.hpp"
#include "CubeMap.hpp"
#include "Cube.hpp"
#include "Camera.h"
#include "model.h"
#include "Plane.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseKey_callback(GLFWwindow* window, int button, int action, int mods);
void do_movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

// Properties
GLuint screenWidth = 800, screenHeight = 800;

GLFWwindow* window;
Cube* cube1;
bool keys[1024];
int xRot, yRot, zRot = 0;
GLfloat fov = 45.0f;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
double lastMouseX, lastMouseY = 0;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool initWindow()
{
	if (!glfwInit())
	{
		std::cout << "ERROR: Could not initialise GLFW...";
		std::cin;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "Assignment #2", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "ERROR: Could not create winodw...";
		std::cin;
		return false;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	glfwSetMouseButtonCallback(window, mouseKey_callback);

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

void mouseKey_callback(GLFWwindow* window, int button, int action, int mods)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	glfwSetCursorPosCallback(window, mouse_callback); 
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
		glfwSetCursorPosCallback(window, NULL); 
		firstMouse = true;
		//glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float deltaX = xpos - lastMouseX;
	float deltaY = lastMouseY - ypos;
	lastMouseX = xpos;
	lastMouseY = ypos;

	camera.ProcessMouseMovement(deltaX, deltaY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset / 100;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

#include "windows.h"
#include "MyForm.h"

using namespace lab001;

public ref class X
{
public:
	static MyForm^ myForm = gcnew MyForm();
	GLfloat refractiveIndex;
	GLfloat mixRatio;
	GLfloat textureRatio;
	bool rotate, normalMap;
	void EntryPoint()
	{
		myForm->refractiveTrackBar->Value = 15;
		Application::EnableVisualStyles();
		Application::Run(myForm);
	}

	void Update(int rotX, int rotY, int rotZ)
	{
		this->refractiveIndex = myForm->refractiveTrackBar->Value / 10.0f;
		this->mixRatio = myForm->ratioTrackBar->Value / 100.0f;
		this->textureRatio = myForm->textureTrackBar->Value / 100.0f;
		this->rotate = myForm->rotateCB->Checked;
		this->normalMap = myForm->normalMapCB->Checked;
	}
};

using namespace System::Threading;

int main()
{
	X^ o1 = gcnew X();
	Thread^ thread = gcnew Thread(gcnew ThreadStart(o1, &X::EntryPoint));
	thread->Start();	
	
	/* Create GL Window */
	if (!initWindow())
		return -1;

	/* Initialise vertex buffers for cube */
	CubeMap cubeMap;
	cubeMap.loadCubeMap("../textures/cubemaps/SwedishRoyalCastle/");

	/* Load shaders needed */
	Shader skyboxShader = *ShaderManager::loadShader("skybox");
	Shader reflectAndRefractShader = *ShaderManager::loadShader("reflectAndRefract");
	Shader lightingShader = *ShaderManager::loadShader("lighting");

	//Model dragonModel("../models/nanosuit/nanosuit.obj");
	//dragonModel.Translate(glm::vec3(0.0f, -1.7f, 0.0f)); // Translate it down a bit so it's at the center of the scene)
	//dragonModel.Scale(glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	//Model dragonModel("../models/monkey_MODEL.dae");
	//Model dragonModel("../models/cube.dae");

	glm::vec3 cube1Position(0.0f, 0.0f, 0.0F);
	cube1 = new Cube(&lightingShader, cube1Position);

	glm::vec3 lampPos(0.0f, 0.0f, 6.0f);
	glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
	glm::vec3 cube1Color(1.0f, 0.5f, 0.31f);
	//Cube lamp(&lampShader, lampPos);
	//lamp.scale(glm::vec3(0.2f));

	Plane plane1(&lightingShader, cube1Position, "../textures/wood.png");

	projectionMatrix = glm::perspective(
		45.0f,
		1.0f,
		0.1f,
		1000.0f
	);
  
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

		/* Rendering Code */
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		do_movement();

		viewMatrix = camera.GetViewMatrix();

		glm::mat4 model;

		reflectAndRefractShader.enableShader();
		reflectAndRefractShader.setUniformMatrix4fv("projection", projectionMatrix);
		reflectAndRefractShader.setUniformMatrix4fv("view", viewMatrix);
		reflectAndRefractShader.setUniformVector3f("cameraPos", camera.Position);
		reflectAndRefractShader.setUniform1f("refractiveIndex", o1->refractiveIndex);
		reflectAndRefractShader.setUniform1f("mixRatio", o1->mixRatio);
		reflectAndRefractShader.setUniform1f("textureRatio", o1->textureRatio);
		reflectAndRefractShader.setUniform1f("normalMap", o1->normalMap);

		//glActiveTexture(GL_TEXTURE3);
		//glUniform1i(glGetUniformLocation(reflectAndRefractShader._shaderProgramID, "skybox"), 3);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.texID);  
		//if (o1->rotate) {
		//	dragonModel.Rotate(glm::vec3(0.00f, 0.01f, 0.00f));
		//}
		//dragonModel.Draw(reflectAndRefractShader);

		lightingShader.enableShader();
		lightingShader.setUniformMatrix4fv("projection", projectionMatrix);
		lightingShader.setUniformMatrix4fv("view", viewMatrix);
		lightingShader.setUniformVector3f("lightColor", lightColor);
		lightingShader.setUniformVector3f("objectColor", cube1Color);
		lightingShader.setUniformVector3f("lightPos", lampPos);
		

		cube1->rotate(glm::vec3(0, 0.01, 0));
		cube1->draw();
		plane1.draw();
	

		skyboxShader.enableShader();
		skyboxShader.setUniformMatrix4fv("projection", projectionMatrix);
		skyboxShader.setUniformMatrix4fv("view", viewMatrix);
		skyboxShader.setUniformMatrix4fv("model", model);



		cubeMap.drawSkyBox(skyboxShader);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
		int xRotAngle = (int)std::abs(((xRot % 630 + 630) / 1.75)) % 360;
		int yRotAngle = (int)std::abs(((yRot % 630 + 630) / 1.75)) % 360;
		int zRotAngle = (int)std::abs(((zRot % 630 + 630) / 1.75)) % 360;
		o1->myForm->Invoke(gcnew Action<int, int, int>(o1, &X::Update), xRotAngle, yRotAngle, zRotAngle);
	}

	glfwTerminate();
	thread->Abort();
	return 0;
}

// Moves/alters the object positions based on user input
void do_movement()
{
	    // Camera controls
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
	if(keys[GLFW_KEY_SPACE])
        camera.ProcessKeyboard(UP, deltaTime);
	if(keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(DOWN, deltaTime);


	if (keys[GLFW_KEY_UP]) {
		yRot--;
		cube1->rotate(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_DOWN]) {
		yRot++;
		cube1->rotate(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_RIGHT]) {
		xRot--;
		cube1->rotate(glm::vec3(0.0f, 0.01f, 0.0f));
	}
	if (keys[GLFW_KEY_LEFT]) {
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