#pragma once

#include "Shader.h"
#include "MonoBehaviour.h"
#include "Texture.h"
#include "Transform.h"
#include "Model.h"

namespace yk
{
	class RenderCom2 : public MonoBehaviour
	{
		TYPE(yk::RenderCom2, yk::MonoBehaviour)
		Shader shader;
		Model* modelobj;
		CTexture tex1;

	public:

		virtual void Awake()
		{

			shader = Shader("1.model_loading.vs", "1.model_loading.fs");

			//modelobj = new Model("model/nanosuit.obj");
			modelobj = new Model("box/box.obj");

			const unsigned int SCR_WIDTH = 800;
			const unsigned int SCR_HEIGHT = 600;
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shader.set("projection", projection);
			glm::mat4 model = glm::mat4(1.0f);
			//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(5.0f, 5, 5));	// it's a bit too big for our scene, so scale it down
			shader.set("model", model);

			tex1.init();
			tex1.load("container.jpg", GL_RGB);
			tex1.use(GL_TEXTURE0);
			shader.set("texture_diffuse1", 0);
		}


		virtual void Update()
		{
			

			auto view = transform()->transformMat();
			shader.set("view", view);
			shader.use();
			modelobj->draw(shader);

		}

	};
}

