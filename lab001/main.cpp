#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <math.h>
#include <sys/timeb.h>
#define _USE_MATH_DEFINES
#include <AntTweakBar.h>

#include "ShaderManager.hpp"
#include "CubeMap.hpp"
#include "Cube.hpp"
#include "Camera.h"
#include "model.h"
#include "Plane.h"
#include "Cone.h"
#include "Animator.h"
#include "WaterFrameBuffers.h"

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouseKey_callback(GLFWwindow* window, int button, int action, int mods);
void do_movement();
void RenderScene(Shader shader, float RunningTime);
int GetMilliCount();

// Camera
Camera camera(glm::vec3(0.0f, 0.5f, 5.0f));

Animator animator;

// Properties
GLuint SCR_WIDTH = 800, SCR_HEIGHT = 800;

GLFWwindow* window;
Cube *cube1, *lamp;
Plane *plane1, *reflectionPlane;
Cone *cone1, *cone2, *cone3;
CubeMap* cubeMap;
Model *mainModel, *bobModel, *swatModel, *handgunModel, *estateModel;
bool keys[1024];
GLfloat fov = 45.0f;
glm::mat4 projectionMatrix;
glm::mat4 viewMatrix;
double lastMouseX, lastMouseY = 0;
bool firstMouse = true;
map<string,glm::quat> KinematicTransforms;	// maps a bone name to its transform

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Shader skyboxShader;
Shader reflectAndRefractShader;
Shader lightingShader;
Shader simpleDepthShader;
Shader shadowShader;

glm::vec3 lightPos(-5.0f, 0.0f, 13.5f);
glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
glm::vec3 cube1Color(1.0f, 0.5f, 0.31f);
float runningSpeed = 860.0f;
float turningSpeed = 0.1f;
float mouseSensitivity = -0.002f;
bool isThirdPersonCamera = false;
long long m_startTime = GetMilliCount();
float currentIntroTime = 0.0f;
float cubeX = -0.60f, cubeY = 1.0f, cubeZ = 0.0f;
int selectedAnimationIndex;
// IK related vars
bool animateCube, b1 = false, b2 = false, b3 = false, drawCones = false;
float kickingBallAnimationTime = 0.0f;
float animationStartTime = 0.0f;

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
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(800, 800, "Assignment #4 (Final) - OpenGL", NULL, NULL);
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

    glfwSetCursorPosCallback(window, mouse_callback);
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

/* This callback resets Intro play time, allowing intro to run again */
void TW_CALL RunIntroCB(void *)
{
    camera.Pitch = 0.0f;
    camera.Yaw = -90.0f;
    currentIntroTime = (GetMilliCount() - m_startTime) / 1000.0f;
    isThirdPersonCamera = false;
}

void initUI()
{
    TwInit(TW_OPENGL_CORE, NULL);
    TwWindowSize(SCR_WIDTH * 2, SCR_HEIGHT * 2);

    TwBar *myBar;
    myBar = TwNewBar("MyTweakBar");

    TwAddVarRW(myBar, "cubeX", TW_TYPE_FLOAT, &cubeX, " min=-10 max=10 step=0.01 group=Cube label='X' ");
    TwAddVarRW(myBar, "cubeY", TW_TYPE_FLOAT, &cubeY, " min=-10 max=10 step=0.01 group=Cube label='Y' ");
    TwAddVarRW(myBar, "cubeZ", TW_TYPE_FLOAT, &cubeZ, " min=-10 max=10 step=0.01 group=Cube label='Z' ");
    TwAddVarRW(myBar, "runningSpeed", TW_TYPE_FLOAT, &runningSpeed, " min=0 max=2500 step=10 label='Running Speed' ");
    TwAddButton(myBar, "Play Intro", RunIntroCB, NULL, " label='Play Intro' ");
    TwAddVarRW(myBar, "3rd person camera", TW_TYPE_BOOLCPP, &isThirdPersonCamera, " label='3rd person camera' ");
    TwAddVarRW(myBar, "Animate cube", TW_TYPE_BOOLCPP, &animateCube, " label='Animate IK Target' ");
    TwAddVarRW(myBar, "Draw cones", TW_TYPE_BOOLCPP, &drawCones, " label='Draw Cones' ");
    TwAddVarRW(myBar, "Hand IK", TW_TYPE_BOOLCPP, &b1, " label='Hand IK' ");
    TwAddVarRW(myBar, "Forearm IK", TW_TYPE_BOOLCPP, &b2, " label='Forearm IK' ");
    TwAddVarRW(myBar, "Upper arm IK", TW_TYPE_BOOLCPP, &b3, " label='Upper arm IK' ");

    // Defining an empty season enum type
    TwType animationType = TwDefineEnum("AnimationType", NULL, 0);
    TwAddVarRW(myBar, "Soldier Animation", animationType, &selectedAnimationIndex, " enum='0 {Idle}, "
            "1 {Idle with Gun}, "
            "2 {Idle with Gun Aiming}, "
            "3 {Idle with gun firing}, "
            "4 {Kneel firing}, "
            "5 {Kneel idle}, "
            "6 {Jump}, "
            "7 {Kick ball}, "
            "8 {Chicken Dance}, "
            "9 {Cat}, "
            "10 {Salsa}, "
            "11 {Run}, "
            "12 {Run with gun}, "
            "13 {T-Pose}, "
            "14 {Walk}, "
            "15 {Walk with gun}' ");
}

void mouseKey_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (!TwEventMouseButtonGLFW(button, action)) {
        // If not handled by AntTweakBar, lock the cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//cout << key << endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstMouse = true;
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
    int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);

    if (cursorMode == GLFW_CURSOR_NORMAL)
    {
        TwEventMousePosGLFW(xpos * 2, ypos * 2);  // send event to AntTweakBar
        // Event was handled by AntTweakBar, return
        return;
    }

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

	if (isThirdPersonCamera) {
		mainModel->Rotate(glm::vec3(0.0f, deltaX * mouseSensitivity, 0.0f));
	}

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

glm::mat4 makeXRotation(double angleInRadians) {
	auto c = cos(angleInRadians);
	auto s = sin(angleInRadians);
	auto m = glm::mat4(
		1, 0, 0, 0, // first column (not row!)
		0, c, -s, 0, // second column
		0, s, c, 0,  // third column
		0, 0, 0, 1
		);

	return m;
}

glm::mat4 makeYRotation(double angleInRadians) {
	auto c = cos(angleInRadians);
	auto s = sin(angleInRadians);
	auto m = glm::mat4(
		c, 0, s, 0, // first column (not row!)
		0, 1, 0, 0, // second column
		-s, 0, c, 0,  // third column
		0, 0, 0, 1
		);

	return m;
}

glm::mat4 makeZRotation(double angleInRadians) {
	auto c = cos(angleInRadians);
	auto s = sin(angleInRadians);
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

	float angle = acos(glm::dot(startVector, endVector));

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
        std::cout << "RBV!" << std::endl;
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

// returns EulerAngles
glm::quat handleRotation(glm::vec3 startVector, glm::vec3 endVector) {
	glm::quat rotateQuaternion = RotationBetweenVectors(startVector, endVector);
	//glm::quat rotateQuaternion = rotateMatrix(startVector, endVector);
	//glm::quat rotateQuaternion = RotationBetweenVectorsThreeJs(startVector, endVector); 
	
	return rotateQuaternion;
}

// handVertex is retrievable from Blender bones head and tail position divided by 100
glm::vec3 boneLocalToGlobal(glm::vec3 vertex, std::string boneName, Model *model) {
	int boneIndex = model->m_BoneMapping[boneName];
	glm::vec4 PosL = model->m_BoneInfo[boneIndex].FinalTransformation * glm::vec4(vertex, 1);
	glm::vec4 PosL2 = model->_modelMatrix * PosL;

	return glm::vec3(PosL2);
}

glm::quat gunInitialRotation(0.707f, 0.0f, -0.707f, 0.0f);

void decomposeBoneMatrix(std::string boneName) {
	int boneIndex = swatModel->m_BoneMapping[boneName];

	glm::vec3 scale, translation, skew;
	glm::vec4 persp;
	glm::quat orientation;
	glm::decompose(swatModel->m_BoneInfo[boneIndex].FinalTransformation, scale, orientation, translation, skew, persp);

	handgunModel->SetRotationFromQuaternion(gunInitialRotation * glm::inverse(orientation));
}

glm::vec3 calculateHandNormal() {
	glm::vec3 handHead = glm::vec3(-0.67416, -0.05412, 1.55149);
	glm::vec3 handTail = glm::vec3(-0.75961, -0.07666, 1.54633);
	auto handHeadG = boneLocalToGlobal(handHead, "Fbx01_R_Hand", mainModel);
	auto handTailG = boneLocalToGlobal(handTail, "Fbx01_R_Hand", mainModel);
	
	return glm::normalize(handTailG - handHeadG);
}

glm::vec3 calculateHandTip() {
	glm::vec3 handTail = glm::vec3(-0.75961, -0.07666, 1.54633);
	auto handTailG = boneLocalToGlobal(handTail, "Fbx01_R_Hand", mainModel);
	
	return handTailG;
}

glm::vec3 calculateSwatHandPosition() {
	glm::vec3 handHead = glm::vec3(-74.024 - 15, 144.573 - 2, -1.482);
	auto handPosition = boneLocalToGlobal(handHead, "swat:RightHand", swatModel);
	return handPosition;
}

glm::vec3 calculateHandPosition() {
	glm::vec3 handHead = glm::vec3(-0.67416, -0.05412, 1.55149);
	auto handPosition = boneLocalToGlobal(handHead, "Fbx01_R_Hand", mainModel);
	return handPosition;
}

glm::quat calculateHandRotation() {
	int boneIndex = mainModel->m_BoneMapping["Fbx01_R_Hand"];
	glm::quat currentQuaternion(mainModel->m_BoneInfo[boneIndex].GlobalTransformation);
	return glm::normalize(currentQuaternion);
}

void drawFirstNormalAndTarget() {
	glm::vec3 startVector = calculateConeNormal();
	glm::vec3 endVector = glm::normalize(cube1->getPosition() - cone1->getPosition());

	handleRotation(startVector, endVector, cone1);

	// hand rotation
	startVector = calculateHandNormal();
    glm::vec3 handPosition = calculateHandPosition();
	endVector = glm::normalize(cube1->getPosition() - handPosition);
	glm::quat currentRotation = KinematicTransforms["Fbx01_R_Hand"];
	glm::quat rotationChange = handleRotation(startVector, endVector);

//    if (abs(cubeY - handPosition.y) < 0.10) {
//        return;
//    }

    if (cubeY <= handPosition.y) {
        float tempX = rotationChange.x;
        rotationChange.x = rotationChange.z;
        rotationChange.z = tempX;
    }

    glm::quat finalRotation = glm::normalize(currentRotation * rotationChange);
	KinematicTransforms["Fbx01_R_Hand"] = finalRotation;

	// hand debug helper arrows
	// glm::vec3 handPosition = calculateHandPosition();
	// firstHandNormal->set(handPosition, handPosition + startVector);
	// firstHandTarget->set(handPosition, handPosition + endVector);
}

glm::vec3 calculateForearmPosition() {
	glm::vec3 forearmHead = glm::vec3(-0.41828, -0.00961, 1.55952);
	auto forearmPosition = boneLocalToGlobal(forearmHead, "Fbx01_R_Forearm", mainModel);
	return forearmPosition;
}

void drawSeconNormalAndTarget() {
	glm::vec3 endVector = glm::normalize(cube1->getPosition() - cone2->getPosition());
	glm::vec3 firstLinkTip = cone1->localToWorld(glm::vec3(0, 1, 0));
	glm::vec3 effectorVector = glm::normalize(firstLinkTip - cone2->getPosition());

	handleRotation(effectorVector, endVector, cone2);

	cone1->setPosition(cone2->localToWorld(glm::vec3(0, 1, 0)));

	// forearm rotation
	glm::vec3 forearmPosition = calculateForearmPosition();
	endVector = glm::normalize(cube1->getPosition() - forearmPosition);
	firstLinkTip = calculateHandTip();
	effectorVector = glm::normalize(firstLinkTip - forearmPosition);

	glm::quat currentRotation = KinematicTransforms["Fbx01_R_Forearm"];
	glm::quat rotationChange = handleRotation(effectorVector, endVector);

//    if (abs(cubeY - forearmPosition.y) < 0.10) {
//        return;
//    }

    if (cubeY <= forearmPosition.y) {
        float tempX = rotationChange.x;
        rotationChange.x = rotationChange.z;
        rotationChange.z = tempX;
    }

	KinematicTransforms["Fbx01_R_Forearm"] = glm::slerp(currentRotation, currentRotation * rotationChange, 0.5f);

	// forearm debug helper arrows
	// forearmNormal->set(forearmPosition, forearmPosition + effectorVector);
	// forearmTarget->set(forearmPosition, forearmPosition + endVector);
}

glm::vec3 calculateUpperArmPosition() {
	glm::vec3 upperArmHead = glm::vec3(-0.19121, -0.03099, 1.57615);
	auto upperArmPosition = boneLocalToGlobal(upperArmHead, "Fbx01_R_UpperArm", mainModel);
	return upperArmPosition;
}

void drawThirdNormalAndTarget() {
	glm::vec3 endVector = glm::normalize(cube1->getPosition() - cone3->getPosition());
	glm::vec3 firstLinkTip = cone1->localToWorld(glm::vec3(0, 1, 0));
	glm::vec3 effectorVector = glm::normalize(firstLinkTip - cone3->getPosition());

	handleRotation(effectorVector, endVector, cone3);

	cone2->setPosition(cone3->localToWorld(glm::vec3(0, 1, 0)));
	cone1->setPosition(cone2->localToWorld(glm::vec3(0, 1, 0)));

	// Upper Arm rotation
	glm::vec3 upperArmPosition = calculateUpperArmPosition();
	endVector = glm::normalize(cube1->getPosition() - upperArmPosition);
	firstLinkTip = calculateHandTip();
	effectorVector = glm::normalize(firstLinkTip - upperArmPosition);

	glm::quat currentRotation = KinematicTransforms["Fbx01_R_UpperArm"];
	glm::quat rotationChange = handleRotation(effectorVector, endVector);

    if (cubeY <= upperArmPosition.y) {
        float tempX = rotationChange.x;
        rotationChange.x = rotationChange.z;
        rotationChange.z = tempX;
    }

	KinematicTransforms["Fbx01_R_UpperArm"] = glm::slerp(currentRotation, currentRotation * rotationChange, 0.5f);

	// Upper Arm debug helper arrows
	// upperArmNormal->set(upperArmPosition, upperArmPosition + effectorVector);
	// upperArmTarget->set(upperArmPosition, upperArmPosition + endVector);
}

void introScene(float AnimationTime) {
    AnimationTime = AnimationTime - currentIntroTime;
    if (AnimationTime > 7.0f) {
        if (AnimationTime < 8.0f) {
            isThirdPersonCamera = true;
        }
        return;
    }

	glm::vec3 translation;
	glm::quat rotation;
	int boneAnimIndex;
	animator.PlayAnimation(0, AnimationTime, translation, rotation, boneAnimIndex, "Camera");
	camera.Position = translation;
	camera.SetRotationFromQuaternion(rotation);

	animator.PlayAnimation(0, AnimationTime, translation, rotation, boneAnimIndex, "Swat");
	swatModel->SetPosition(translation);
    swatModel->SetRotationFromQuaternion(rotation);
    swatModel->SetAnimationIndex(boneAnimIndex);
}

int main()
{
	/* Create GL Window */
	if (!initWindow())
		return -1;

    /* Init AntTweakBar UI */
    initUI();

	/* Initialise vertex buffers for cube */
	cubeMap = new CubeMap();
	cubeMap->loadCubeMap("../textures/cubemaps/ThinMatrix/");

	/* Load shaders needed */
	skyboxShader = *ShaderManager::loadShader("skybox");
	reflectAndRefractShader = *ShaderManager::loadShader("reflectAndRefract");
	lightingShader = *ShaderManager::loadShader("lighting");
	simpleDepthShader = *ShaderManager::loadShader("simpleDepth");
	shadowShader = *ShaderManager::loadShader("shadow");

	// Setup correct textures
	shadowShader.enableShader();
	glUniform1i(glGetUniformLocation(shadowShader._shaderProgramID, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shadowShader._shaderProgramID, "shadowMap"), 2);

	//Model mainModel("../models/nanosuit/nanosuit.obj");
	//mainModel.Translate(glm::vec3(0.0f, -1.7f, 0.0f)); // Translate it down a bit so it's at the center of the scene)
	//mainModel.Scale(glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	//mainModel = new Model("../models/Bonanza.dae");
	//mainModel->Scale(glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
	//mainModel->Rotate(glm::vec3(1.0f, 0, 0));	// It's a bit too big for our scene, so scale it down
	//mainModel = new Model("../models/cube.dae");
	//mainModel = new Model("../models/monkey_MODEL.dae");
	//mainModel = new Model("../models/finger.dae");
	//mainModel->Rotate(glm::vec3(0, 0, M_PI/2));
	//mainModel->Translate(glm::vec3(5.0f, 0.0f, 0.0f));

	//bobModel = new Model("../models/bob.fbx");
	//bobModel->Translate(glm::vec3(-2.0f, 0.0f, 0.0f));
	//bobModel->Scale(glm::vec3(0.00025f, 0.00025f, 0.00025f));	// It's a bit too big for our scene, so scale it down


	//mainModel = new Model("../models/boblampclean.md5mesh");
	//mainModel = new Model("../models/marine_anims.dae");
	mainModel = new Model("../models/marine_anims.fbx");
	mainModel->Scale(glm::vec3(0.0001f));	// It's a bit too big for our scene, so scale it down
    mainModel->SetPosition(glm::vec3(-10.0f, 0.0f, 13.5f));
    mainModel->Rotate(glm::vec3(0, M_PI/2, 0));

	swatModel = new Model("../models/Swat.fbx");
	swatModel->Scale(glm::vec3(0.01f));	// It's a bit too big for our scene, so scale it down

	handgunModel = new Model("../models/handgun.fbx");
    handgunModel->Scale(glm::vec3(1.1f));

	estateModel = new Model("../models/cs_estate/cs_estate.fbx");

	KinematicTransforms["Fbx01_R_UpperArm"] = glm::quat();
	KinematicTransforms["Fbx01_R_Forearm"] = glm::quat();
	//KinematicTransforms["Fbx01_R_Hand"] = glm::quat(0.4768f, 0.5266f, -0.4412f, 0.5480f);
    KinematicTransforms["Fbx01_R_Hand"] = glm::quat();

	glm::vec3 cube1Position(cubeX, cubeY, cubeZ);
	cube1 = new Cube(&lightingShader, cube1Position);
    cube1->scale(glm::vec3(0.1f));

	plane1 = new Plane(&shadowShader, glm::vec3(0.0f, 2.0f, 0.0f), "../textures/wood.png");

	cone1 = new Cone(&shadowShader);
	cone1->setPosition(glm::vec3(2.0f, 6.0f, 0.0f));

	cone2 = new Cone(&shadowShader);
	cone2->setPosition(glm::vec3(2.0f, 5.0f, 0.0f));

	cone3 = new Cone(&shadowShader);
	cone3->setPosition(glm::vec3(2.0f, 4.0f, 0.0f));
	

	lamp = new Cube(&shadowShader, lightPos);
	lamp->scale(glm::vec3(0.2f));

	//************************* Water Renderer Setup **************************/
	WaterFrameBuffers* fbos = new WaterFrameBuffers();
	reflectionPlane = new Plane(&lightingShader, glm::vec3(1.0f, 1.0f, 0.0f), fbos->getReflectionTexture());

	projectionMatrix = glm::perspective(
		45.0f,
		1.0f,
		0.1f,
		1000.0f
	);
  
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time, time in fractions of a second
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (kickingBallAnimationTime > 0)
            kickingBallAnimationTime -= deltaTime;

		if (b1) 
			drawFirstNormalAndTarget();
		if (b2)
			drawSeconNormalAndTarget();
		if (b3)
			drawThirdNormalAndTarget();

        if (animateCube) {
            cubeX = sin(currentFrame/2) * 2;
            cubeY = sin(currentFrame/3) * 2 + 2;
            cubeZ = sin(currentFrame/4) * 2;
        }

        cube1->setPosition(glm::vec3(cubeX, cubeY, cubeZ));

		/* Rendering Code */
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		do_movement();
		camera.IsThirdPersonCamera = isThirdPersonCamera;

		// Camera animation script
		float RunningTime = (float)((double)GetMilliCount() - (double)m_startTime) / 1000.0f;
		introScene(RunningTime);

		viewMatrix = camera.GetViewMatrix(mainModel->GetYRotation(), mainModel->GetPosition());

        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL) {
            mainModel->SetAnimationIndex(selectedAnimationIndex);
        }

		// Water simulation
		fbos->bindReflectionFrameBuffer();
		RenderScene(shadowShader, RunningTime);
		fbos->unbindCurrentFrameBuffer();

		// 2. Render scene as normal 
        glViewport(0, 0, 1600, 1600);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderScene(shadowShader, RunningTime);

		// Draw skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.enableShader();
		skyboxShader.setUniformMatrix4fv("projection", projectionMatrix);
		skyboxShader.setUniformMatrix4fv("view", glm::mat4(glm::mat3(viewMatrix)));
		cubeMap->drawSkyBox(skyboxShader);
		glDepthFunc(GL_LESS);

        TwDraw();  // draw the tweak bar(s)

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	fbos->cleanUp();
    TwTerminate();
	glfwTerminate();
	return 0;
}

// Grabbed it here: http://www.firstobject.com/getmillicount-milliseconds-portable-c++.htm
int GetMilliCount()
{
    // Something like GetTickCount but portable
    // It rolls over every ~ 12.1 days (0x100000/24/60/60)
    // Use GetMilliSpan to correct for rollover
    timeb tb;
    ftime( &tb );
    int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
    return nCount;
}

void RenderScene(Shader shader, float RunningTime)
{
    RunningTime = RunningTime - animationStartTime;
	shader.enableShader();
	shader.setUniformMatrix4fv("projection", projectionMatrix);
	shader.setUniformMatrix4fv("view", viewMatrix);
	shader.setUniformVector3f("lightPos", lightPos);
	shader.setUniformVector3f("viewPos", camera.Position);
	shader.setUniform1f("shadows", 1);

	vector<glm::mat4> Transforms;
	//bobModel->BoneTransform(RunningTime, Transforms, KinematicTransforms);
	//bobModel->Draw(shader, Transforms);

    mainModel->enableKinematics = (b1 || b2 || b3);
    mainModel->BoneTransform(RunningTime, Transforms, KinematicTransforms);
    mainModel->Draw(shader, Transforms);

    swatModel->BoneTransform(RunningTime, Transforms, KinematicTransforms);
    swatModel->Draw(shader, Transforms);

	// Draw in wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::vec3 swatHandPosition = calculateSwatHandPosition();
	handgunModel->SetPosition(swatHandPosition);
	decomposeBoneMatrix("swat:RightHand");
	handgunModel->Draw(shader);

	estateModel->Draw(shader);

	//shader.setUniformMatrix4fv("model", plane1->_modelMatrix);
	//plane1->draw();

	shader.setUniformMatrix4fv("model", reflectionPlane->_modelMatrix);
	reflectionPlane->draw();

	shader.setUniformMatrix4fv("model", cube1->_modelMatrix);
    shader.setUniform1f("utilityColor", 1);
	cube1->draw();	

    if (drawCones) {
        shader.setUniformMatrix4fv("model", cone1->_modelMatrix);
        cone1->draw();

        shader.setUniformMatrix4fv("model", cone2->_modelMatrix);
        cone2->draw();

        shader.setUniformMatrix4fv("model", cone3->_modelMatrix);
        cone3->draw();
    }


    shader.setUniformMatrix4fv("model", lamp->_modelMatrix);

    glm::vec3 playerPos = mainModel->GetPosition();
    // Check the distance between ball and player
    if (abs(playerPos.x - lightPos.x) < 3 && abs(playerPos.z - lightPos.z) < 3) {
        // If he's nearby and the animation is almost over => make the ball fly
        if (kickingBallAnimationTime > 0 && kickingBallAnimationTime < 2) {
            lamp->move(mainModel->GetForwardVector());
        }
        // If the animation is over, reset ball position
        if (kickingBallAnimationTime <= 0) {
            lamp->setPosition(lightPos);
        }
    }
    lamp->draw();

    shader.setUniform1f("utilityColor", 0);
	// Disable wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

// Moves/alters the object positions based on user input
void do_movement()
{
	// Camera controls
    if(keys[GLFW_KEY_UP])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_DOWN])
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if(keys[GLFW_KEY_LEFT])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_RIGHT])
        camera.ProcessKeyboard(RIGHT, deltaTime);
	if(keys[GLFW_KEY_SPACE])
        camera.ProcessKeyboard(UP, deltaTime);
	if(keys[GLFW_KEY_LEFT_SHIFT])
        camera.ProcessKeyboard(DOWN, deltaTime);

    if (kickingBallAnimationTime > 0) {
        return;
    }
	// reset the animation if no keys are held
	mainModel->SetAnimationIndex(0);

    if (keys[GLFW_KEY_Q]) {
        // Kick ball
        kickingBallAnimationTime = 5;
        animationStartTime = (GetMilliCount() - m_startTime) / 1000.0f;
        mainModel->SetAnimationIndex(7);
    }

	if (keys[GLFW_KEY_W]) {
		mainModel->SetAnimationIndex(11);
		mainModel->Translate(glm::vec3(0.0f, 0.0f, runningSpeed));
	}
	if (keys[GLFW_KEY_S]) {
		mainModel->SetAnimationIndex(11);
		mainModel->Translate(glm::vec3(0.0f, 0.0f, -runningSpeed));
	}
	if (keys[GLFW_KEY_A]) {
		mainModel->SetAnimationIndex(14);
		mainModel->Translate(glm::vec3(runningSpeed / 2, 0.0f, 0.0f));
	}
	if (keys[GLFW_KEY_D]) {
		mainModel->SetAnimationIndex(14);
		mainModel->Translate(glm::vec3(-runningSpeed / 2, 0.0f, 0.0f));
	}
}