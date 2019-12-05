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
#include "FrameBufferCom.h"
#include "RenderMgr.h"
#include "LightCom.h"
#include "Transform.h"

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


void SetQuadMesh(shared_ptr<MeshFilter> filter)
{
	auto mesh = make_shared<Mesh>();
	
	vector<vec3> verts = {
		vec3(-1, -1, 0),
		vec3(1, -1, 0),
		vec3(1, 1, 0),
		vec3(-1, 1, 0),
	};

	vector<vec2> texcoords = {
		vec2(0, 0),
		vec2(1, 0),
		vec2(1, 1),
		vec2(0, 1)
	};

	vector<unsigned int> ind = {
		0, 1, 2,
		0, 2, 3
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = verts[i];
		v.texCoord = texcoords[i];
		mesh->AddVert(move(v));
	}

	mesh->AddIndices(ind);
	filter->mesh = mesh;
}

void SetCubeMapMesh(shared_ptr <MeshFilter> filter)
{
	vector<float> skyboxVertices = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};
	vector<unsigned int> indices;
	auto mesh = make_shared<Mesh>();

	for (int i = 0; i < skyboxVertices.size() / 3; i++)
	{
		Vertex v;
		v.position = vec3(
			skyboxVertices[i * 3],
			skyboxVertices[i * 3 + 1],
			skyboxVertices[i * 3 + 2]
		);
		mesh->AddVert(move(v));
	}

	for (unsigned int i = 0; i < skyboxVertices.size(); i++)
	{
		indices.push_back(i);
	}
	mesh->AddIndices(indices);
	filter->mesh = mesh;
}

void SetMat(shared_ptr<MeshRenderer> render)
{
	Shader s = Shader("5.1.framebuffers_screen.vs", "5.1.framebuffers_screen.fs");
	auto mat = make_shared<Material>(s);
	render->setMaterial(mat);
}

void SetCubeMapMat(shared_ptr<MeshRenderer> render)
{
	Shader s = Shader("cubemap.vert", "cubemap.frag");
	auto mat = make_shared<Material>(s);
	render->setMaterial(mat);
}

void SceneMgr::UpdateScene(Timer& timer)
{
	currentScene()->UpdateGameObjects();
	currentScene()->FillMatricesUbo();
	currentScene()->FillLightsUbo();
	RenderMgr::RenderUpdate(currentScene());
	currentScene()->LateUpdateGameObjects();
}

/*
测试用场景，用于生成场景内物体
*/
shared_ptr<Scene> SceneMgr::testScene()
{
	// 创建并设置当前场景
	auto scene = make_shared<Scene>();
	m_curScene = scene;

	// 创建模型游戏对象步骤
	// 1.创建 Shader, Material
	// 2.创建 GameObject
	// 2.添加 ModelFilter, ModelRenderer 组件并初始化
	/*auto role = scene->CreateGameObject();
	{
		auto shader = Shader("1.model_loading.vert", "1.model_loading.fs");
		auto mat = make_shared<Material>(shader);

		auto filter = role->AddComponent<ModelFilter>();
		auto render = role->AddComponent<ModelRenderer>();

		filter->Load("model/nanosuit.obj");
		render->setMaterial(mat);
	}*/

	auto cubeRole = scene->CreateGameObject();
	{
		auto cubeMf = cubeRole->AddComponent<MeshFilter>();
		cubeMf->mesh = make_shared<Mesh>(Mesh::CubeMesh());
		auto texID = TextureLoader::TextureFromFile("container.jpg");

		Texture t;
		t.id = texID;
		t.type = "texture_diffuse";
		t.path = "container.jpg";
		cubeMf->mesh->textures.push_back(t);
		auto cubeRd = cubeRole->AddComponent<MeshRenderer>();
		//auto cubeShader = Shader("2.1.basic_lighting.vert", "2.1.basic_lighting.frag");
		auto cubeShader = Shader("standard.vert", "standard.frag");

		auto mat = make_shared<Material>(cubeShader);
		cubeRd->setMaterial(mat);
	}

	auto pointLight = scene->CreateGameObject();
	{
		auto light = pointLight->AddComponent<LightCom>();
		light->lightMode() = LightMode::POINT_LIGHT;
		auto mesh = Mesh::CubeMesh();
		light->transform()->position() = vec3(0, 0, 0);
		auto mf = pointLight->AddComponent<MeshFilter>();
		mf->mesh = make_shared<Mesh>(mesh);

		auto mr = pointLight->AddComponent<MeshRenderer>();
		mr->useDefaultMaterial();
	}
	

	// 创建相机步骤
	// 1. 创建 GameObject
	// 2. 添加 Camera 组件控制显示
	// 3. 添加 CamMove 组件
	// 4. 设置场景相机

	auto mainCamera = scene->CreatePerspectiveCamera();
	mainCamera->AddComponent<CamMove>();
	

	// 控制离屏渲染的对象
	auto renderObj = CreatePostEffectObject();
	auto frameCom = renderObj->AddComponent<FrameBufferCom>();
	auto meshFilter = renderObj->AddComponent<MeshFilter>();
	auto meshRender = renderObj->AddComponent<MeshRenderer>();
	SetQuadMesh(meshFilter);
	SetMat(meshRender);

	Texture t;
	t.id = frameCom->tex;
	t.type = "texture_diffuse";
	meshFilter->mesh->textures.push_back(move(t));	// 设置贴图为颜色缓冲

	// cubemap对象
	// rendermgr中单独渲染天空盒
	// mesh中支持设置cubemap
	auto cubemapobj = scene->CreateCubeMap(vector<string>{
			"cubemap/skybox/right.jpg",
			"cubemap/skybox/left.jpg",
			"cubemap/skybox/top.jpg",
			"cubemap/skybox/bottom.jpg",
			"cubemap/skybox/front.jpg",
			"cubemap/skybox/back.jpg"
	});
	scene->SetCubeMap(cubemapobj);
	//auto cubemapobj = CreateCubeMapObject();
	/*auto cubeMeshFilter = cubemapobj->AddComponent<MeshFilter>();
	auto cubeMeshRender = cubemapobj->AddComponent<MeshRenderer>();

	SetCubeMapMesh(cubeMeshFilter);
	SetCubeMapMat(cubeMeshRender);

	auto cubeMapID = TextureLoader::TextureCubeMap(vector<string>{
				"cubemap/skybox/right.jpg", 
				"cubemap/skybox/left.jpg", 
				"cubemap/skybox/top.jpg", 
				"cubemap/skybox/bottom.jpg", 
				"cubemap/skybox/front.jpg", 
				"cubemap/skybox/back.jpg"
	});

	Texture cubeTex;
	cubeTex.id = cubeMapID;
	cubeTex.type = "texture_cubemap";
	cubeMeshFilter->mesh->textures.push_back(move(cubeTex));*/

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

shared_ptr<GameObject> SceneMgr::CreatePostEffectObject()
{
	auto gb = GameObject::Create();
	if (m_curScene)
	{
		m_curScene->SetPostEffect(gb);
	}
	return gb;
}

shared_ptr<GameObject> SceneMgr::CreateCubeMapObject()
{
	auto gb = GameObject::Create();
	if (m_curScene)
	{
		m_curScene->SetCubeMap(gb);
	}
	return gb;
}

