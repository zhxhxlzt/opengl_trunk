#pragma once
#include "MonoBehaviour.h"
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"

#include "Mesh.h"
#include "Model.h"
namespace yk
{
	class BoxCom : public MonoBehaviour
	{
		TYPE(yk::BoxCom, MonoBehaviour)
		shared_ptr<Model> m;
		Shader shader;
	public:
		virtual void Awake()
		{
			m = make_shared<Model>("model/nanosuit.obj");
			shader.initByPath("1.model_loading.vert", "1.model_loading.fs");
		}

		virtual void Update()
		{
			auto cam = SceneMgr::currentScene()->mainCamera();
			mat4 model = transform()->Model();
			mat4 view = cam->transform()->View();
			mat4 projection = cam->Proj();
			shader.use();
			shader.set("projection", projection);
			shader.set("model", model);
			shader.set("view", view);
			m->draw(shader);
		}
	};
}