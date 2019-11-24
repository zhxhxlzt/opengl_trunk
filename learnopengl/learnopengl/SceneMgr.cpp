#include "SceneMgr.h"
#include "GameObject.h"
#include "Camera.h"
#include "Mesh.h"
#include "MeshFilter.h"
#include "Texture.h"
#include "MeshRenderer.h"
#include "ModelRenderer.h"
#include "RenderCom2.h"
#include "CamMove.h"
#include "BoxCom.h"
#include "ModelFilter.h"

using namespace yk;
shared_ptr<Scene> SceneMgr::m_curScene = nullptr;


shared_ptr<GameObject> box()
{
	auto box = GameObject::Create();
	auto mf = box->AddComponent<MeshFilter>();
	mf->mesh = Model::box();
	auto rd = box->AddComponent<MeshRenderer>();
	
	return box;
}

shared_ptr<GameObject> RenderGameObject()
{
	auto obj = GameObject::Create();
	obj->AddComponent<ModelRenderer>();
	return obj;
}

void SceneMgr::load(shared_ptr<Scene> scene)
 {
	 m_curScene = scene;
 }

//shared_ptr<Scene> SceneMgr::testScene()
//{
//	m_curScene = make_shared<Scene>();
//	auto actor = SceneMgr::CreateGameObject();
//	actor->AddComponent<BoxCom>();
//
//	auto camobj = SceneMgr::CreateGameObject();
//	camobj->transform()->position() = vec3(0, 5, 30);
//	camobj->AddComponent<CamMove>();
//	auto cam_Comp = camobj->AddComponent<Camera>();
//
//
//	m_curScene->SetMainCamera(cam_Comp);
//	return m_curScene;
//}


shared_ptr<Scene> SceneMgr::testScene()
{
	// 创建并设置当前场景
	auto scene = make_shared<Scene>();
	m_curScene = scene;

	// 创建模型游戏对象步骤
	// 1.创建 Shader, Material
	// 2.创建 GameObject
	// 2.添加 ModelFilter, ModelRenderer 组件并初始化
	auto shader = Shader("1.model_loading.vert", "1.model_loading.fs");
	auto mat = make_shared<Material>(shader);
	
	auto role = CreateGameObject();
	auto filter = role->AddComponent<ModelFilter>();
	auto render = role->AddComponent<ModelRenderer>();

	filter->Load("model/nanosuit.obj");
	render->setMaterial(mat);

	// 创建相机步骤
	// 1. 创建 GameObject
	// 2. 添加 Camera 组件控制显示
	// 3. 添加 CamMove 组件
	// 4. 设置场景相机

	auto mainCamera = CreateGameObject();
	mainCamera->transform()->position() = vec3(0, 7, 15);
	auto camComp = mainCamera->AddComponent<Camera>();
	camComp->SetFov(110.0f);
	camComp->SetNear(0.01f);
	camComp->SetFar(100.0f);

	auto camMove = mainCamera->AddComponent<CamMove>();

	scene->SetMainCamera(camComp);
	return scene;
}

shared_ptr<GameObject> SceneMgr::CreateGameObject()
{
	auto gb = GameObject::Create();
	if (m_curScene)
	{
		m_curScene->AddGameObject(gb);
	}
	return gb;
}