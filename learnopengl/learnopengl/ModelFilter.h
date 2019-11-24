#pragma once
#include "Component.h"
#include "Model.h"

namespace yk
{
	class ModelFilter : public Component
	{
		TYPE(yk::ModelFilter, Component)
	public:
		shared_ptr<Model> model() { return m_model; }
		void Load(string path)
		{
			m_model = make_shared<Model>(path);
		}
		vector<Mesh> GetMeshes()
		{
			return m_model->getMeshes();
		}
		Mesh GetMesh(unsigned int idx)
		{
			return m_model->getMesh(idx);
		}

	private:
		shared_ptr<Model> m_model;
	};


}