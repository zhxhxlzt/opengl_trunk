#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexcoord;

// æÿ’Û–≈œ¢
layout (std140, binding=0) uniform matrices
{
	mat4 view;
	mat4 proj;
	vec3 camPos;
};

out ArrayInfo
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec3 CamPos;
} info;

uniform mat4 model;

void main()
{
	gl_Position = proj * view * model * vec4(aPos, 1);
	info.FragPos = vec3(model * vec4(aPos, 1));
	info.Normal = aNormal;
	info.TexCoord = aTexcoord;
	info.CamPos = camPos;
}
