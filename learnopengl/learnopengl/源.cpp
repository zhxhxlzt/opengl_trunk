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
#include "MonoBehaviour.h"
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
	auto gb = GameObject::createGameObject();
	gb->AddComponent<MonoBehaviour>();
	gb->AddComponent<Component>();

	Application app;
	app.StartTimer(testfunc, 3000);
	app.run();
	return 0;
}