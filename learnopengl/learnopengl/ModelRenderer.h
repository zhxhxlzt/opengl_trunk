#pragma once
#include "Renderer.h"
#include "Model.h"

namespace yk
{
	class ModelRenderer : public Renderer
	{
		TYPE(yk::ModelRenderer, Renderer)
	public:
		virtual void Rendering();


	private:
		shared_ptr<Model> m_model;
	};
}