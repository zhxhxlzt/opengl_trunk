#version 330 core
out vec4 FragColor;

//in vec2 TexCoords;

//uniform sampler2D texture_diffuse1;

void main()
{    
    //FragColor = texture(texture_diffuse1, TexCoords);
	FragColor = vec4(0.5f, 0.7f, 0.3f, 1.0f);
}