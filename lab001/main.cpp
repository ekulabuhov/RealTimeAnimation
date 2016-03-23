#include <iostream>

#include <GL\glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <math.h>
#define _USE_MATH_DEFINES

#include "ShaderManager.hpp"
#include "CubeMap.hpp"
#include "Cube.hpp"
#include "Camera.h"
#include "model.h"
#include "Plane.h"
#include "Cone.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseKey_callback(GLFWwindow* window, int button, int action, int mods);
void do_movement();
void RenderScene(Shader shader);

// Camera
Camera camera(glm::vec3(3.0f, 0.5f, 5.0f));

// Properties
GLuint SCR_WIDTH = 800, SCR_HEIGHT = 800;

GLFWwindow* window;
Cube *cube1, *lamp;
Plane* plane1;
Cone *cone1, *cone2, *cone3;
CubeMap* cubeMap;
Model* dragonModel;
bool keys[1024];
int xRot, yRot, zRot = 0;
GLfloat fov = 45.0f;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
double lastMouseX, lastMouseY = 0;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Shader skyboxShader;
Shader reflectAndRefractShader;
Shader lightingShader;
Shader simpleDepthShader;
Shader shadowShader;

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 cube1Color(1.0f, 0.5f, 0.31f);

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

	window = glfwCreateWindow(800, 800, "Assignment #3 (IK)", NULL, NULL);
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
	float posX, posY, posZ;
	bool animate;
	void EntryPoint()
	{
		Application::EnableVisualStyles();
		Application::Run(myForm);
	}

	void Update()
	{
		this->posX = (myForm->posXTrackBar->Value - 100) / 10.0f;
		myForm->lblAngleX->Text = "posX: " + this->posX;
		
		this->posY = (myForm->posYTrackBar->Value + 10) / 20.0f;
		myForm->lblAngleY->Text = "posY: " + this->posY;
		
		this->posZ = (myForm->posZTrackBar->Value - 100) / 10.0f;
		myForm->lblAngleZ->Text = "posZ: " + this->posZ;

		this->animate = myForm->cbAnimate->Checked;
		
		/*this->refractiveIndex = myForm->refractiveTrackBar->Value / 10.0f;
		this->mixRatio = myForm->ratioTrackBar->Value / 100.0f;
		this->textureRatio = myForm->textureTrackBar->Value / 100.0f;
		this->rotate = myForm->rotateCB->Checked;
		this->normalMap = myForm->normalMapCB->Checked;*/
	}
};

glm::mat4 makeXRotation(double angleInRadians) {
	auto c = Math::Cos(angleInRadians);
	auto s = Math::Sin(angleInRadians);
	auto m = glm::mat4(
		1, 0, 0, 0, // first column (not row!)
		0, c, -s, 0, // second column
		0, s, c, 0,  // third column
		0, 0, 0, 1
		);

	return m;
}

glm::mat4 makeYRotation(double angleInRadians) {
	auto c = Math::Cos(angleInRadians);
	auto s = Math::Sin(angleInRadians);
	auto m = glm::mat4(
		c, 0, s, 0, // first column (not row!)
		0, 1, 0, 0, // second column
		-s, 0, c, 0,  // third column
		0, 0, 0, 1
		);

	return m;
}

glm::mat4 makeZRotation(double angleInRadians) {
	auto c = Math::Cos(angleInRadians);
	auto s = Math::Sin(angleInRadians);
	auto m = glm::mat4(
		c, -s, 0, 0, // first column (not row!)
		s, c, 0, 0, // second column
		0, 0, 1, 0,  // third column
		0, 0, 0, 1
		);

	return m;
}

// Returns a rotation needed to align 2 vectors (v1)
// Straight port from JS code
glm::quat rotateMatrix(glm::vec3 startVector, glm::vec3 endVector) {
	glm::vec3 axis;
	glm::quat quaternion;

	float angle = Math::Acos(glm::dot(startVector, endVector));

	if (angle == angle) {

		axis = glm::normalize(glm::cross(startVector, endVector));
		quaternion = glm::angleAxis(angle, axis);
	}

	return quaternion;
}

// Returns a rotation needed to align 2 vectors (v2)
// Took it from here: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest){
	start = glm::normalize(start);
	dest = glm::normalize(dest);

	float cosTheta = glm::dot(start, dest);
	glm::vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
			rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(180.0f, rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return glm::quat(
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}

void handleRotation(glm::vec3 startVector, glm::vec3 endVector, Geometry *obj) {
	glm::quat rotateQuaternion = RotationBetweenVectors(startVector, endVector);
	glm::quat curQuaternion = obj->quaternion;

	curQuaternion = rotateQuaternion * curQuaternion;
	glm::normalize(curQuaternion);

	obj->setRotationFromQuaternion(curQuaternion);
}

glm::vec3 calculateConeNormal() {
	return glm::normalize(cone1->getUpVector());
}

void drawFirstNormalAndTarget() {
	glm::vec3 startVector = calculateConeNormal();
	glm::vec3 endVector = glm::normalize(cube1->getPosition() - cone1->getPosition());

	handleRotation(startVector, endVector, cone1);
}

void drawSeconNormalAndTarget() {
	glm::vec3 endVector = glm::normalize(cube1->getPosition() - cone2->getPosition());
	glm::vec3 firstLinkTip = cone1->localToWorld(glm::vec3(0, 1, 0));
	glm::vec3 effectorVector = glm::normalize(firstLinkTip - cone2->getPosition());

	handleRotation(effectorVector, endVector, cone2);

	cone1->setPosition(cone2->localToWorld(glm::vec3(0, 1, 0)));
}

void drawThirdNormalAndTarget() {
	glm::vec3 endVector = glm::normalize(cube1->getPosition() - cone3->getPosition());
	glm::vec3 firstLinkTip = cone1->localToWorld(glm::vec3(0, 1, 0));
	glm::vec3 effectorVector = glm::normalize(firstLinkTip - cone3->getPosition());

	handleRotation(effectorVector, endVector, cone3);

	cone2->setPosition(cone3->localToWorld(glm::vec3(0, 1, 0)));
	cone1->setPosition(cone2->localToWorld(glm::vec3(0, 1, 0)));
}

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
	cubeMap = new CubeMap();
	cubeMap->loadCubeMap("../textures/cubemaps/SwedishRoyalCastle/");

	/* Load shaders needed */
	skyboxShader = *ShaderManager::loadShader("skybox");
	reflectAndRefractShader = *ShaderManager::loadShader("reflectAndRefract");
	lightingShader = *ShaderManager::loadShader("lighting");
	simpleDepthShader = *ShaderManager::loadShader("simpleDepth");
	shadowShader = *ShaderManager::loadShader("shadow");

	// Setup correct textures
	shadowShader.enableShader();
	glUniform1i(glGetUniformLocation(shadowShader._shaderProgramID, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shadowShader._shaderProgramID, "shadowMap"), 1);

	//Model dragonModel("../models/nanosuit/nanosuit.obj");
	//dragonModel.Translate(glm::vec3(0.0f, -1.7f, 0.0f)); // Translate it down a bit so it's at the center of the scene)
	//dragonModel.Scale(glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	//dragonModel = new Model("../models/Bonanza.dae");
	//dragonModel->Scale(glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	//dragonModel->Rotate(glm::vec3(1.0f, 0, 0));	// It's a bit too big for our scene, so scale it down
	//dragonModel = new Model("../models/cube.dae");
	//dragonModel = new Model("../models/monkey_MODEL.dae");

	glm::vec3 cube1Position(5.0f, 0.5f, 0.0F);
	cube1 = new Cube(&shadowShader, cube1Position);

	cone1 = new Cone(&shadowShader);
	cone1->setPosition(glm::vec3(2.0f, 6.0f, 0.0f));

	cone2 = new Cone(&shadowShader);
	cone2->setPosition(glm::vec3(2.0f, 5.0f, 0.0f));

	cone3 = new Cone(&shadowShader);
	cone3->setPosition(glm::vec3(2.0f, 4.0f, 0.0f));
	

	lamp = new Cube(&shadowShader, lightPos);
	lamp->scale(glm::vec3(0.2f));

	glm::vec3 plane1Position(0.0f, 0.0f, 0.0F);
	plane1 = new Plane(&shadowShader, plane1Position, "../textures/wood.png");

	// Configure depth map FBO
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // - Create depth texture
    GLuint depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



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

		if (o1->animate) {
			glm::vec3 cubePosition(Math::Sin(currentFrame/2) * 4, Math::Sin(currentFrame/3) * 4 + 5, Math::Sin(currentFrame/4) * 4);
			cube1->setPosition(cubePosition);
		} else {
			cube1->setPosition(glm::vec3(o1->posX/1.0f, o1->posY/1.0f, o1->posZ/1.0f));
		}

		drawFirstNormalAndTarget();
		drawSeconNormalAndTarget();
		drawThirdNormalAndTarget();

		/* Rendering Code */
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		do_movement();
		viewMatrix = camera.GetViewMatrix();

		// 1. Render depth of scene to texture (from light's perspective)
        // - Get light projection/view matrix.
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        GLfloat near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(1.0));
        lightSpaceMatrix = lightProjection * lightView;
		
        // - render scene from light's point of view
		simpleDepthShader.enableShader();
		simpleDepthShader.setUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            RenderScene(simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//viewMatrix = lightSpaceMatrix;
		// 2. Render scene as normal 
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowShader.enableShader();
        // Set light uniforms 
		shadowShader.setUniformMatrix4fv("lightSpaceMatrix", lightSpaceMatrix);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
		RenderScene(shadowShader);

		skyboxShader.enableShader();
		skyboxShader.setUniformMatrix4fv("projection", projectionMatrix);
		skyboxShader.setUniformMatrix4fv("view", viewMatrix);
		skyboxShader.setUniformMatrix4fv("model", glm::mat4());

		cubeMap->drawSkyBox(skyboxShader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
		o1->myForm->Invoke(gcnew Action(o1, &X::Update) /*, xRotAngle, yRotAngle, zRotAngle*/);
	}

	glfwTerminate();
	thread->Abort();
	return 0;
}

void RenderScene(Shader shader)
{
	shader.enableShader();
	shader.setUniformMatrix4fv("projection", projectionMatrix);
	shader.setUniformMatrix4fv("view", viewMatrix);
	shader.setUniformVector3f("lightPos", lightPos);
	shader.setUniformVector3f("viewPos", camera.Position);
	shader.setUniform1f("shadows", 1);

	//dragonModel->Draw(shader);

	//cube1->rotate(glm::vec3(0, 0.01, 0));
	shader.setUniformMatrix4fv("model", cube1->_modelMatrix);
	cube1->draw();

	
	// Draw in wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	shader.setUniformMatrix4fv("model", cone1->_modelMatrix);
	cone1->draw();

	shader.setUniformMatrix4fv("model", cone2->_modelMatrix);
	cone2->draw();

	shader.setUniformMatrix4fv("model", cone3->_modelMatrix);
	cone3->draw();
	// Disable wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	shader.setUniformMatrix4fv("model", plane1->_modelMatrix);
	plane1->draw();
	shader.setUniformMatrix4fv("model", lamp->_modelMatrix);
	lamp->draw();
}

void DrawDragonModel()
{
	//reflectAndRefractShader.enableShader();
	//reflectAndRefractShader.setUniformMatrix4fv("projection", projectionMatrix);
	//reflectAndRefractShader.setUniformMatrix4fv("view", viewMatrix);
	//reflectAndRefractShader.setUniformVector3f("cameraPos", camera.Position);
	//reflectAndRefractShader.setUniform1f("refractiveIndex", o1->refractiveIndex);
	//reflectAndRefractShader.setUniform1f("mixRatio", o1->mixRatio);
	//reflectAndRefractShader.setUniform1f("textureRatio", o1->textureRatio);
	//reflectAndRefractShader.setUniform1f("normalMap", o1->normalMap);

	//glActiveTexture(GL_TEXTURE3);
	//glUniform1i(glGetUniformLocation(reflectAndRefractShader._shaderProgramID, "skybox"), 3);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap.texID);  
	//if (o1->rotate) {
	//	dragonModel.Rotate(glm::vec3(0.00f, 0.01f, 0.00f));
	//}
	//dragonModel.Draw(reflectAndRefractShader);
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
		dragonModel->Rotate(glm::vec3(0.01f, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_DOWN]) {
		yRot++;
		dragonModel->Rotate(glm::vec3(-0.01f, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_RIGHT]) {
		xRot--;
		dragonModel->Rotate(glm::vec3(0.0f, 0.01f, 0.0f));
	}
	if (keys[GLFW_KEY_LEFT]) {
		xRot++;
		dragonModel->Rotate(glm::vec3(0.0f, -0.01f, 0.0f));
	}
	if (keys[GLFW_KEY_Q]) {
		zRot++;
		dragonModel->Rotate(glm::vec3(0.0f, 0.0f, 0.01f));
	}
	if (keys[GLFW_KEY_E]) {
		zRot--;
		dragonModel->Rotate(glm::vec3(0.0f, 0.0f, -0.01f));
	}

	if (keys[GLFW_KEY_M]) {
		zRot--;
		dragonModel->Rotate(glm::vec3(0.0f, 0.0f, -0.01f));
	}
}