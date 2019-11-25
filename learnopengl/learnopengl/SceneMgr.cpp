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
	auto filter = make_shared<MeshFilter>();
	
	vector<vec3> verts = {
		vec3(0, 0, 0),
		vec3(1, 0, 0),
		vec3(1, 1, 0),
		vec3(0, 1, 0),
	};

	vector<unsigned int> ind = {
		0, 1, 2,
		0, 2, 3
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex v;
		v.position = verts[i];
		mesh->AddVert(move(v));
	}

	mesh->AddIndices(ind);
	filter->mesh = mesh;
}

shared_ptr<Material> GetMat()
{
	Shader s = Shader("5.1.framebuffers_screen.vs", "5.1.framebuffers_screen.fs");
	auto mat = make_shared<Material>(s);
	return mat;
}

void SetMat(shared_ptr<MeshRenderer> render)
{
	auto mat = GetMat();
	render->setMaterial(mat);
}

shared_ptr<Scene> SceneMgr::testScene()
{
	// ���������õ�ǰ����
	auto scene = make_shared<Scene>();
	m_curScene = scene;

	// ����ģ����Ϸ������
	// 1.���� Shader, Material
	// 2.���� GameObject
	// 2.��� ModelFilter, ModelRenderer �������ʼ��
	auto shader = Shader("1.model_loading.vert", "1.model_loading.fs");
	auto mat = make_shared<Material>(shader);
	
	auto role = CreateGameObject();
	auto filter = role->AddComponent<ModelFilter>();
	auto render = role->AddComponent<ModelRenderer>();

	filter->Load("model/nanosuit.obj");
	render->setMaterial(mat);

	// �����������
	// 1. ���� GameObject
	// 2. ��� Camera ���������ʾ
	// 3. ��� CamMove ���
	// 4. ���ó������

	auto mainCamera = CreateGameObject();
	mainCamera->transform()->position() = vec3(0, 7, 15);
	auto camMove = mainCamera->AddComponent<CamMove>();
	auto camComp = mainCamera->AddComponent<Camera>();
	camComp->SetFov(110.0f);
	camComp->SetNear(0.01f);
	camComp->SetFar(100.0f);
	scene->SetMainCamera(camComp);

	// ����������Ⱦ�Ķ���
	auto renderObj = CreateGameObject();
	auto frameCom = renderObj->AddComponent<FrameBufferCom>();
	auto meshFilter = renderObj->AddComponent<MeshFilter>();
	auto meshRender = renderObj->AddComponent<MeshRenderer>();
	SetQuadMesh(meshFilter);
	SetMat(meshRender);

	Texture t;
	t.id = frameCom->tex;
	t.type = "texture_diffuse";
	meshFilter->mesh->textures.push_back(move(t));	// ������ͼΪ��ɫ����

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