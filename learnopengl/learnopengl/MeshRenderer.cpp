#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "GameObject.h"
using namespace yk;

void MeshRenderer::Rendering()
{
	//auto mesh_filter = getComponent<MeshFilter>();
	auto mesh_filter = gameObject()->GetComponent<MeshFilter>();
	//cout << "mesh_filter" << mesh_filter->GetInstanceID() << endl;
	shared_ptr<Mesh> tar_mesh;
	if (mesh_filter->mesh)
		tar_mesh = mesh_filter->mesh;
	else
		tar_mesh = mesh_filter->sharedMesh;

	auto shader = material()->shader();
	tar_mesh->draw(shader);
}