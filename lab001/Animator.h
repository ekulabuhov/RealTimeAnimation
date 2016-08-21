#pragma once

// Std. Includes
#include <vector>
#include <math.h>

// GL Includes
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct BoneAnimation {
	int animationIndex;
	int frame;

	void set(int animationIndex, int frame) {
		this->animationIndex = animationIndex;
		this->frame = frame;
	}
};

struct PositionTransform {
	glm::vec3 position;
	int frame;

	void set(glm::vec3 position, int frame) {
		this->position = position;
		this->frame = frame;
	}
};

struct RotationTransform {
	glm::quat rotation;
	int frame;

	void set(glm::quat rotation, int frame) {
		this->rotation = rotation;
		this->frame = frame;
	}
};

struct Transformations {
	std::vector<PositionTransform> position;
	std::vector<RotationTransform> rotation;
	std::vector<BoneAnimation> boneAnimation;
};

class Animator
{
public:
	map<string,Transformations> tracks;	// maps an actor to timeline

	Animator(void)
	{
		Transformations cameraFrames;
		cameraFrames.position.resize(6);
		cameraFrames.position[0].set(glm::vec3(10.0f, 3.76f, -9.0f), 0);
		cameraFrames.position[1].set(glm::vec3(10.0f, 3.76f, 1.87f), 60);
		cameraFrames.position[2].set(glm::vec3(-1.79f, 3.76f, 1.87f), 80);
		cameraFrames.position[3].set(glm::vec3(-1.79f, 3.76f, 1.87f), 121);
		cameraFrames.position[4].set(glm::vec3(-1.79f, 3.76f, 14.0f), 170);
		cameraFrames.position[5].set(glm::vec3(-1.79f, 3.76f, 14.0f), 211);

		cameraFrames.rotation.resize(4);
		cameraFrames.rotation[0].set(glm::quat(0.995f, 0.10f, 0.0f, 0.0f), 0);
		cameraFrames.rotation[1].set(glm::quat(0.995f, 0.10f, 0.0f, 0.0f), 120);
		cameraFrames.rotation[2].set(glm::quat(0.704f, 0.068f, -0.704f, -0.068f), 190);
		cameraFrames.rotation[3].set(glm::quat(0.704f, 0.068f, -0.704f, -0.068f), 211);

		tracks["Camera"] = cameraFrames;

		Transformations swatFrames;
		swatFrames.position.resize(8);
		swatFrames.position[0].set(glm::vec3(6.50f, 0.00f, -16.62f), 0);
		swatFrames.position[1].set(glm::vec3(10.0f, 0.00f, -16.62f), 25);
		swatFrames.position[2].set(glm::vec3(10.0f, 0.00f, -7.86f), 50);
		swatFrames.position[3].set(glm::vec3(10.0f, 0.00f, -4.38f), 60);
		swatFrames.position[4].set(glm::vec3(1.71f, 0.00f, -1.80f), 90);
		swatFrames.position[5].set(glm::vec3(-0.63f, 0.00f, -1.80f), 100);
		swatFrames.position[6].set(glm::vec3(-0.63f, 0.93f, -4.57f), 115);
		swatFrames.position[7].set(glm::vec3(-0.63f, 0.93f, -4.57f), 211);

		swatFrames.rotation.resize(8);
		swatFrames.rotation[0].set(glm::quat(0.707f, 0.0f, 0.707f, 0.0f), 0);
		swatFrames.rotation[1].set(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 25);
		swatFrames.rotation[2].set(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 50);
		swatFrames.rotation[3].set(glm::quat(0.707f, 0.0f, -0.707f, 0.0f), 60);
		swatFrames.rotation[4].set(glm::quat(0.87f, 0.0f, -0.492f, 0.0f), 90);
		swatFrames.rotation[5].set(glm::quat(0.0f, 0.0f, -1.0f, 0.0f), 100);
		swatFrames.rotation[6].set(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 115);
		swatFrames.rotation[7].set(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 211);

		swatFrames.boneAnimation.resize(3);
		swatFrames.boneAnimation[0].set(1, 0);
		swatFrames.boneAnimation[1].set(0, 115);
		swatFrames.boneAnimation[2].set(0, 211);

		tracks["Swat"] = swatFrames;
	}

	void CalcInterpolatedRotation(glm::quat& Out, GLuint animationFrame, std::vector<RotationTransform> frames)
	{
		// we need at least two values to interpolate...
		if (frames.size() == 1) {
			Out = frames[0].rotation;
			return;
		}
    
		GLuint RotationIndex = FindPosition(animationFrame, frames);
		GLuint NextRotationIndex = (RotationIndex + 1);
		assert(NextRotationIndex < frames.size());
		float DeltaTime = (float)(frames[NextRotationIndex].frame - frames[RotationIndex].frame);
		float Factor = (animationFrame - (float)frames[RotationIndex].frame) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const glm::quat& StartRotationQ = frames[RotationIndex].rotation;
		const glm::quat& EndRotationQ   = frames[NextRotationIndex].rotation;
		Out = glm::slerp(StartRotationQ, EndRotationQ, Factor);
		Out = glm::normalize(Out);
	}

	GLuint FindPosition(GLuint animationFrame, std::vector<RotationTransform> frames) {
		for (GLuint i = 0; i < frames.size() - 1; i++) {
			if (animationFrame < frames[i + 1].frame) {
				return i;
			}
		}

		assert(0);

		return 0;
	}

	GLuint FindPosition(GLuint animationFrame, std::vector<PositionTransform> frames)
	{
		for (GLuint i = 0; i < frames.size() - 1; i++) {
			if (animationFrame < frames[i + 1].frame) {
				return i;
			}
		}

		assert(0);

		return 0;
	}

	void CalcInterpolatedPosition(glm::vec3& Out, GLuint animationFrame, std::vector<PositionTransform> frames)
	{
		GLuint PositionIndex = FindPosition(animationFrame, frames);
		GLuint NextPositionIndex = (PositionIndex + 1);
		float DeltaTime = (float)(frames[NextPositionIndex].frame - frames[PositionIndex].frame);
		// We calculate the influence of two positions based on how close we are to timestamp
		float Factor = (animationFrame - (float)frames[PositionIndex].frame) / DeltaTime;
		assert(Factor >= 0.0f && Factor <= 1.0f);
		const glm::vec3& Start = frames[PositionIndex].position;
		const glm::vec3& End = frames[NextPositionIndex].position;
		glm::vec3 Delta = End - Start;
		Out = Start + Factor * Delta;
	}

	void GetBoneAnimationIndex(int& boneAnimIndex, GLuint animationFrame, std::vector<BoneAnimation> frames) 
	{
		for (GLuint i = 0; i < frames.size() - 1; i++) {
			if (animationFrame < frames[i + 1].frame) {
				boneAnimIndex = frames[i].animationIndex;
				return;
			}
		}
	}

	void PlayAnimation(int animationIndex, float animationTime, glm::vec3& Translation, glm::quat& Rotation, int& boneAnimIndex, string trackName) {		
		// This makes animation loop at the end
		// animationTime = fmod(animationTime, 7.0f);
		GLuint animationFrame = animationTime * 30;

		Transformations frames = tracks[trackName];

		// Interpolate translation and generate translation transformation matrix
		CalcInterpolatedPosition(Translation, animationFrame, frames.position);

		if (frames.rotation.size() > 0) {
			CalcInterpolatedRotation(Rotation, animationFrame, frames.rotation);
		}

		if (frames.boneAnimation.size() > 0) {
			GetBoneAnimationIndex(boneAnimIndex, animationFrame, frames.boneAnimation);
		}
	}
};

