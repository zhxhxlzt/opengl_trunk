#pragma once
#include "Component.h"
#include "Material.h"
#include "MeshFilter.h"

namespace yk
{
	class Renderer : public Component
	{
		TYPE(yk::Renderer, Component);

	public:
		virtual void Rendering() {}

		shared_ptr<Material> material()
		{
			return mat;
		}

		void setMaterial(shared_ptr<Material> mat)
		{
			this->mat = mat;
		}

		void useDefaultMaterial()
		{
			Shader shader = Shader("standard.vert", "standard.frag");
			mat = make_shared<Material>(shader);
		}

	protected:
		shared_ptr<Material> mat;
	};
}