#pragma once

#include "Component.h"
#include "Mesh.h"
namespace yk
{
	class MeshFilter : public Component
	{
		TYPE(yk::MeshFilter, yk::Component);

	public:
		shared_ptr<Mesh> mesh;
		shared_ptr<Mesh> sharedMesh;
	};
}