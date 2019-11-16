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
#include "Window.h"
#include "SceneMgr.h"


using namespace std;
using namespace yk;

class A
{
public:
	static vector<int> vc;
};

vector<int> A::vc;

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
	SceneMgr scene;
	scene.init();	// ´°¿ÚÉú³É
	scene.testscene();
	Application app;
	app.StartTimer(bind(&SceneMgr::update, &scene, placeholders::_1));
	app.run();
	return 0;
}