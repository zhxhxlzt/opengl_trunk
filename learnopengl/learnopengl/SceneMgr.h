#pragma once
#include "Object.h"
#include "Scene.h"
#include "Timer.h"
namespace yk
{
	// 管理场景内gameobject生命周期，渲染流程
	class SceneMgr : public Object
	{
		TYPE(yk::SceneMgr, Object)
	public:
		static void load(shared_ptr<Scene> scene);

		static shared_ptr<Scene> testScene();

		static void UpdateScene(Timer& timer);

		static shared_ptr<Scene> currentScene() { return m_curScene; }

		static shared_ptr<GameObject> CreateGameObject();

		static shared_ptr<GameObject> CreateCubeMapObject();

		static shared_ptr<GameObject> CreatePostEffectObject();

	private:
		static shared_ptr<Scene> m_curScene;
	};
}