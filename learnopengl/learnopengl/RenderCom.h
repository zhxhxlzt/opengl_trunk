#pragma once

#include "Shader.h"
#include "MonoBehaviour.h"
#include "Texture.h"


namespace yk
{
	class RenderCom : public MonoBehaviour
	{
		const char *vertexShaderSource = "#version 330 core\n"
			"layout (location = 0) in vec3 aPos;\n"
			"layout (location = 1) in vec3 aColor;\n"
			"out vec3 ourColor;\n"
			"void main()\n"
			"{\n"
			"   gl_Position = vec4(aPos, 1.0);\n"
			"   ourColor = aColor;\n"
			"}\0";

		const char *fragmentShaderSource = "#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec3 ourColor;\n"
			"void main()\n"
			"{\n"
			"   FragColor = vec4(ourColor, 1.0f);\n"
			"}\n\0";

		float vertices[32] = {
			// positions          // colors           // texture coords
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
		};

		unsigned int indices[6] = {
			  0, 1, 3, // first triangle
			  1, 2, 3  // second triangle
		};
		Shader shader;

	public:

		virtual void Awake()
		{
			shader = Shader("4.2.texture.vertex", "4.2.texture.frag");
			/*shader.init(vertexShaderSource, fragmentShaderSource);*/
			shader.use();
			unsigned int VBO, VAO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// color attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// texture coord attribute
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);

			Texture tex1, tex2;
			tex1.init();
			tex2.init();

			tex1.load("container.jpg", GL_RGB);
			tex2.load("awesomeface.png", GL_RGBA);

			tex1.use(GL_TEXTURE0);
			tex2.use(GL_TEXTURE1);

			shader.set("texture1", 0);
			shader.set("texture2", 1);
		}


		virtual void Update()
		{
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

	};
}

