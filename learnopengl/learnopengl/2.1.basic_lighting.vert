#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (std140, binding=0) uniform matrices
{
	mat4 view1;
	mat4 projection1;
};


void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;  
	TexCoord = aTexCoord;
    
    gl_Position = projection1 * view1 * vec4(FragPos, 1.0);
}