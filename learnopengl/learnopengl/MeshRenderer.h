#pragma once

#include "Renderer.h"


namespace yk
{
	class MeshRenderer : public Renderer
	{
		TYPE(yk::MeshRenderer, Renderer)
	public:
		virtual void Rendering();
	};
}