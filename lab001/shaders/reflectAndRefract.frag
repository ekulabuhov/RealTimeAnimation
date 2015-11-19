#version 330 core
in vec3 Normal;
in vec3 Position;
in vec2 TexCoords;

uniform vec3 cameraPos;
uniform float refractiveIndex;
uniform float mixRatio;
uniform float textureRatio;
uniform sampler2D texture_diffuse1;
uniform samplerCube skybox;
uniform bool normalMap;

out vec4 color;

void main()
{
	// Diffuse
    vec4 diffuse_color = texture(texture_diffuse1, TexCoords);
	
	// Reflect    
	vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec4 reflectColor = texture(skybox, R);
	
	// Refract         
    float ratio = 1.00 / refractiveIndex;
    R = refract(I, normalize(Normal), ratio);
    vec4 refractColor = texture(skybox, R);

	color = mix(reflectColor, refractColor, mixRatio); 
	color = mix(diffuse_color, color, textureRatio);

	if (normalMap) {
		color = vec4(normalize(Normal), 1.0);
	}
}  