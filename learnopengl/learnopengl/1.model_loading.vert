#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (std140, binding = 0) uniform matrices
{
	mat4 view1;
	mat4 projection1;
};

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection1 * view1 * model * vec4(aPos, 1.0);
}