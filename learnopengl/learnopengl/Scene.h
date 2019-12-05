#pragma once

#include <algorithm>
#include "Object.h"
#include "GameObject.h"
#include "Camera.h"
#include "MeshRenderer.h"
#include "LightCom.h"
/*
framebuffer 绑定规则
index 0: view mat4 proj mat4 eye vec3 size: 16N + 16N + 4N
index 1: 定向光 { 方向 vec3，强度 float，颜色 vec3 } 4N + N + 4N
index 2: 点光   { 数量 uint, { 位置 vec3 强度 float 颜色 vec3 } * 10

*/
namespace yk
{
	class Scene : public Object
	{
		TYPE(yk::Scene, Object);

	public:
		Scene() :Object()
		{
			// view proj 矩阵
			glGenBuffers(1, &m_matricesUBO);
			glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2 + sizeof(vec4), NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_matricesUBO);

			// 定向光
			glGenBuffers(1, &m_dirLightUbo);
			glBindBuffer(GL_UNIFORM_BUFFER, m_dirLightUbo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float) + sizeof(vec4), NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_dirLightUbo);

			//点光
			glGenBuffers(1, &m_pointLightsUbo);
			glBindBuffer(GL_UNIFORM_BUFFER, m_pointLightsUbo);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*10, NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_pointLightsUbo);
		}
		void FillMatricesUbo()
		{
			// 填充view，projection矩阵
			glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), value_ptr(m_mainCamera->transform()->View()));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), value_ptr(m_mainCamera->Proj()));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, sizeof(vec3), value_ptr(m_mainCamera->transform()->position()));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		void FillLightsUbo()
		{
			vector<shared_ptr<LightCom>> pointLights;
			for (auto &e : GameObjects())
			{
				auto light = e->GetComponent<LightCom>();
				if (light)
				{
					if (light->lightMode() == LightMode::DIR_LIGHT)
					{
						glBindBuffer(GL_UNIFORM_BUFFER, m_dirLightUbo);
						glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(vec3), value_ptr(light->dir()));
						glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4), sizeof(float), &light->intensity());
						glBufferSubData(GL_UNIFORM_BUFFER, sizeof(vec4) + sizeof(float), sizeof(vec3), value_ptr(light->color()));
						glBindBuffer(GL_UNIFORM_BUFFER, 0);
					}
					else if (light->lightMode() == LightMode::POINT_LIGHT)
					{
						pointLights.push_back(light);
					}
				}
			}

			glBindBuffer(GL_UNIFORM_BUFFER, m_pointLightsUbo);
			auto size = pointLights.size();
			unsigned offset = sizeof(unsigned);
			unsigned step = sizeof(vec4) * 2 + sizeof(float);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(unsigned int), &size);

			for (auto &light : pointLights)
			{
				//glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(vec3), &light->transform()->position());
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(unsigned int), sizeof(vec3), value_ptr(vec3(0.4, 0.5, 0.7)));

				cout << "pos:" << light->transform()->position() << endl;
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(unsigned int) + sizeof(vec4), sizeof(float), &(light->intensity()));
				glBufferSubData(GL_UNIFORM_BUFFER, sizeof(unsigned int) + sizeof(vec4) + sizeof(float), sizeof(vec3), value_ptr(light->color()));
				offset += step;
			}
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
		void UpdateGameObjects()
		{
			for (auto &entry : m_gameObjects)
			{
				auto gb = entry.second;
				if (gb->active)
				{
					gb->UpdateComponent();
				}
			}
		}

		void LateUpdateGameObjects()
		{
			for (auto &entry : m_gameObjects)
			{
				auto gb = entry.second;
				if (gb->active)
				{
					gb->LateUpdateComponent();
				}
			}
		}
		
		shared_ptr<GameObject> CreateGameObject()
		{
			auto gb = GameObject::Create();
			m_gameObjects[gb->GetInstanceID()] = gb;
			return gb;
		}
		
		shared_ptr<GameObject> CreatePerspectiveCamera()
		{
			auto gb = CreateGameObject();
			gb->transform()->position() =vec3(0, 7, 15);			

			auto camComp = gb->AddComponent<Camera>();
			camComp->SetFov(110.0f);
			camComp->SetNear(0.01f);
			camComp->SetFar(100.0f);
			
			if (not m_mainCamera)
			{
				m_mainCamera = camComp;
			}
			return gb;
		}

		void SetSkyBox(shared_ptr<GameObject> box)
		{
			m_cubemapObj = box;
		}

		shared_ptr<GameObject> CreateCubeMap(vector<string> faces)
		{
			auto gb = GameObject::Create();
			auto mf = gb->AddComponent<MeshFilter>();
			auto mr = gb->AddComponent<MeshRenderer>();
			SetCubeMapMesh(mf);
			SetCubeMapMaterial(mr);
			mf->mesh->textures.push_back(genCubeMapTexture(faces));
			return gb;
		}

		vector<shared_ptr<GameObject>> GameObjects()
		{
			vector<shared_ptr<GameObject>> gameobjects;
			for (auto e : m_gameObjects)
				gameobjects.push_back(e.second);

			return move(gameobjects);
		}

		bool AddGameObject(shared_ptr<GameObject> gb)
		{
			if (m_gameObjects.find(gb->GetInstanceID()) == m_gameObjects.end())
			{
				m_gameObjects.insert_or_assign(gb->GetInstanceID(), gb);
				return true;
			}
			return false;
		}

		bool RemoveGameObject(shared_ptr<GameObject> gb)
		{
			if (m_gameObjects.find(gb->GetInstanceID()) != m_gameObjects.end())
			{
				m_gameObjects.erase(gb->GetInstanceID());
				return true;
			}
			return false;
		}

		void SetCubeMap(shared_ptr<GameObject> cubeMapObj) { m_cubemapObj = cubeMapObj; }

		shared_ptr<GameObject> cubeMapObject() { return m_cubemapObj; }

		void SetPostEffect(shared_ptr<GameObject> postEffectObj) { m_postEffectObj = postEffectObj; }

		shared_ptr<GameObject> postEffectObject(){ return m_postEffectObj; }

	public:
		shared_ptr<Camera> mainCamera()
		{
			return m_mainCamera;
		}

		void SetMainCamera(shared_ptr<Camera> cam)
		{
			if (cam)
				m_mainCamera = cam;
		}

	private:
		unsigned int m_matricesUBO;
		unsigned int m_dirLightUbo;
		unsigned int m_pointLightsUbo;

		shared_ptr<Camera> m_mainCamera;
		map<unsigned int, shared_ptr<GameObject>> m_gameObjects;
		shared_ptr<GameObject> m_cubemapObj;
		shared_ptr<GameObject> m_postEffectObj;

	private:
		Texture genCubeMapTexture(vector<string> faces)
		{
			auto cubeMapID = TextureLoader::TextureCubeMap(faces);
			Texture cubeTex;
			cubeTex.id = cubeMapID;
			cubeTex.type = "texture_cubemap";
			return cubeTex;
		}
		void SetCubeMapMesh(shared_ptr<MeshFilter> meshFilter)
		{
			vector<float> skyboxVertices = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};
			vector<unsigned int> indices;
			auto mesh = make_shared<Mesh>();

			for (int i = 0; i < skyboxVertices.size() / 3; i++)
			{
				Vertex v;
				v.position = vec3(
					skyboxVertices[i * 3],
					skyboxVertices[i * 3 + 1],
					skyboxVertices[i * 3 + 2]
				);
				mesh->AddVert(move(v));
			}

			for (unsigned int i = 0; i < skyboxVertices.size(); i++)
			{
				indices.push_back(i);
			}
			mesh->AddIndices(indices);
			
			meshFilter->mesh = mesh;
		}
		void SetCubeMapMaterial(shared_ptr<MeshRenderer> meshRender)
		{
			Shader s = Shader("cubemap.vert", "cubemap.frag");
			auto mat = make_shared<Material>(s);
			meshRender->setMaterial(mat);
		}
	};
}