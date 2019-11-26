#pragma once

#include <algorithm>
#include "Object.h"
#include "GameObject.h"
#include "Camera.h"

namespace yk
{
	class Scene : public Object
	{
		TYPE(yk::Scene, Object);

	public:
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
		shared_ptr<Camera> m_mainCamera;
		map<unsigned int, shared_ptr<GameObject>> m_gameObjects;
		shared_ptr<GameObject> m_cubemapObj;
		shared_ptr<GameObject> m_postEffectObj;
	};
}