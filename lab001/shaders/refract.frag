#version 330 core
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform float refractiveIndex;

out vec4 color;

void main()
{             
    float ratio = 1.00 / refractiveIndex;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);
    color = texture(skybox, R);
}  