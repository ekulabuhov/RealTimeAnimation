#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

const int MAX_BONES = 100;
uniform mat4 gBones[MAX_BONES];

void main()
{
	mat4 BoneTransform = mat4(1.0);
	// EK: this check disables bone transformation for meshes that lack bones
	if (Weights[0] != 0) {
		BoneTransform	   = gBones[BoneIDs[0]] * Weights[0];
		BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
		BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
		BoneTransform     += gBones[BoneIDs[3]] * Weights[3];
	}

	vec4 PosL					= BoneTransform * vec4(position, 1.0f);
    gl_Position					= lightSpaceMatrix * model * PosL;
}