#include "ModelRenderer.h"
#include "SceneMgr.h"
#include "Transform.h"
#include "GameObject.h"
#include "ModelFilter.h"
using namespace yk;


void ModelRenderer::Rendering()
{
	// ��Ⱦģ�Ͳ���
	// 1. ��ȡshader
	// 2. ��ȡģ��
	// 3. ��ȡtransform����������ת������
	// 4. ��ȡ�������Ĺ۲�ռ䣬ͶӰ�ռ�ת������
	// 5. ����shader��model, view, projection����
	// 6. draw

	auto shader = material()->shader();
	
	auto filter = gameObject()->GetComponent<ModelFilter>();
	auto actor = filter->model();

	// LINK 2019���󣬲���ʹ��Component���е�GetComponent����
	//auto filter = GetComponent<ModelFilter>();
	//auto actor = filter->model();

	auto modelMatrix = transform()->Model();
	auto cam = SceneMgr::currentScene()->mainCamera();
	auto viewMatrix = cam->transform()->View();
	auto projMatrix = cam->Proj();

	shader.use();
	shader.set("model",			modelMatrix);
	shader.set("view",			viewMatrix);
	shader.set("projection",	projMatrix);
	//cout << glGetError() << endl;
	actor->draw(shader);
}