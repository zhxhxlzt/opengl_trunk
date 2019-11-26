#version 330 core

uniform sampler2D ftex;

in vec2 Tex;

out vec4 FragColor;
void main()
{
	//vec3 col = vec3(texture(ftex, Tex));
	FragColor = vec4(1, 0, 1, 1);
}