#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec4 clipSpace;
out vec2 textureCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

const float tiling = 6.0;

void main()
{
    clipSpace = projection * view * model * vec4(position, 1.0);
	gl_Position = clipSpace;
	textureCoords = texCoords;
}