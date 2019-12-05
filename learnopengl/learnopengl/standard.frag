#version 420 core

// æÿ’Û–≈œ¢
layout (std140, binding=0) uniform matrices
{
	mat4 view;
	mat4 proj;
	vec3 camPos;
};

layout (std140, binding=1) uniform dirLight
{
	vec3 dir;
	float intensity;
	vec3 color;
};

struct pointLightInfo
{
	vec3 pos;
	float intensity;
	vec3 color;
};

in ArrayInfo
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
	vec3 CamPos;
} info;

layout (std140, binding=2) uniform pointLight
{
	unsigned int count;
	//pointLightInfo pInfo[10];
	vec3 mycolor;
	float xy;
	vec3 good;
};

uniform sampler2D texture_diffuse1;

out vec4 FragColor;

void main()
{
//	vec3 objectcolor = vec3(texture(texture_diffuse1, info.TexCoord));
	vec3 objectcolor = vec3(0.4, 0.5, 0.2);
	vec3 ambient = 0.1 * objectcolor;//* pInfo[0].color;
	vec3 pos2light = normalize(mycolor);
	float diffuseFactor = max(dot(pos2light, normalize(info.Normal)), 0);
	vec3 diffuse = diffuseFactor * objectcolor;// * pInfo[0].color;
	//vec3 color =  pInfo[0].color * info.FragPos;
	//FragColor = vec4(ambient + diffuse,1);// * vec4(0.5, 0.2, 0.7, 0,9);
	//FragColor = vec4(normalize(pos2light), 1);
	FragColor = vec4(mycolor, 1);
//	FragColor = vec4(1, 0, 0, 1);
}
