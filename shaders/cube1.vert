#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 modelMat;

void main()
{
	gl_Position = modelMat * vec4(position.x + 1.0f, position.y, position.z, 1.0);
}