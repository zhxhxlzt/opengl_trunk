#pragma once

#include "Component.h"
#include "Shader.h"
#include "Model.h"
#include "Texture.h"

namespace yk
{
	class Material : public Object
	{
		TYPE(yk::Material, Object);

	public:
		Material()
		{
			m_shader = standardShader();
		}
		Material(Shader shader) : Object()
		{
			m_shader = shader;
		}
		Shader shader() { return m_shader; }
		Texture mainTex() { return m_mainTexture; }
		void SetShader(Shader shader) { m_shader = shader; }
		void SetMainTex(Texture tex) { m_mainTexture = tex; }
	private:
		Shader m_shader;
		Texture m_mainTexture;

	public:
		static Material get()
		{
			Shader shader = Shader("1.model_loading.vert", "1.model_loading.fs");
			CTexture t;
			t.load("container.jpg", GL_RGB);

			Texture tex;
			tex.id = t.getTextureID();
			tex.type = "texture_diffuse";
			tex.path = "container.jpg";

			return Material(shader);
		}

		static Shader standardShader()
		{
			Shader s;
			s.initByPath("1.model_loading.vert", "1.model_loading.fs");
			return s;
		}
	};
}
