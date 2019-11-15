#include<stdio.h>
#include<vector>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<assimp/camera.h>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"Object.h"
#include"Component.h"
#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
using namespace std;
using namespace yk;

class A
{
public:
	
};

void print_v(vec3 v)
{
	cout << v.x << " " << v.y << " " << v.z << endl;
}

void testfunc()
{
	printf("running !\n");
}

int main()
{
	Application app;
	app.StartTimer(testfunc, 1000);
	app.run();
	return 0;
}