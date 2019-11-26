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
		TYPE(yk::RenderCom2, MonoBehaviour)
		Shader shader;
		Model* modelobj;
		TextureLoader tex1;
		mat4 lastview = mat4(1);

	public:

		virtual void Awake()
		{

			//shader = Shader("test.vert", "test.frag");
			shader = Shader("1.model_loading.vert", "1.model_loading.fs");


			modelobj = new Model("model/nanosuit.obj");
			//modelobj = new Model("box/box.obj");

			const unsigned int SCR_WIDTH = 800;
			const unsigned int SCR_HEIGHT = 600;
			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
			shader.set("projection", projection);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
			model = glm::scale(model, glm::vec3(5.0f, 5, 5));	// it's a bit too big for our scene, so scale it down
			shader.set("model", model);

			tex1.init();
			tex1.load("container.jpg", GL_RGB);
			tex1.use(GL_TEXTURE0);
			shader.set("texture_diffuse1", tex1.getTextureID());
		}


		virtual void Update()
		{
			// 如果不重新设置model，view矩阵，会导致着色器中没有这两项数据 
			shader.use();
			glm::mat4 model = glm::mat4(1.0f);
			shader.set("model", model);

			glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
			shader.set("projection", projection);

			auto tr = transform();

			auto view = translate(model, vec3(0, 0, 4));

			cout << view << endl;

			shader.set("view", view);

			modelobj->draw(shader);
			glCheckError();
		}

	};
}

