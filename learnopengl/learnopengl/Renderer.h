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

	protected:
		shared_ptr<Material> mat;
	};
}