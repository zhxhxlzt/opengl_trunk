#include "ModelRenderer.h"
#include "SceneMgr.h"
#include "Transform.h"
#include "GameObject.h"
#include "ModelFilter.h"
using namespace yk;


void ModelRenderer::Rendering()
{
	// 渲染模型步骤
	// 1. 获取shader
	// 2. 获取模型
	// 3. 获取transform的世界坐标转换矩阵
	// 4. 获取相机对象的观察空间，投影空间转换矩阵
	// 5. 设置shader的model, view, projection参数
	// 6. draw

	auto shader = material()->shader();
	
	auto filter = gameObject()->GetComponent<ModelFilter>();
	auto actor = filter->model();

	// LINK 2019错误，不能使用Component类中的GetComponent方法
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