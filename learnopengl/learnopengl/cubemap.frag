#version 330 core
in vec3 TexCoords;
out vec4 FragColor;

uniform samplerCube texture_cubemap1;

void main()
{
	FragColor = texture(texture_cubemap1, TexCoords);	
}