#include "MeshRenderer.h"
#include "MeshFilter.h"
#include "Mesh.h"
#include "GameObject.h"
#include "GraphicDebug.h"
#include "SceneMgr.h"

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

	auto modelMatrix = transform()->Model();
	auto cam = SceneMgr::currentScene()->mainCamera();
	auto viewMatrix = cam->transform()->View();
	auto projMatrix = cam->Proj();

	shader.use();
	shader.set("model", modelMatrix);
	shader.set("view", viewMatrix);
	shader.set("projection", projMatrix);

	tar_mesh->draw(shader);
	glCheckError();
}