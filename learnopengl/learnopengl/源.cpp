﻿#include<stdio.h>
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


int main()
{
	SceneMgr scene;
	scene.init();	// 窗口生成
	scene.testscene();
	Application app;
	app.StartTimer(bind(&SceneMgr::update, &scene, placeholders::_1));
	auto perp = glm::perspective(60, 1, 10, 20);

	app.run();
	return 0;
}