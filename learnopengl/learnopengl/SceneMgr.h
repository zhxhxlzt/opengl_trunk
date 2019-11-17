#pragma once
#include "Object.h"
#include "Window.h"
#include "Timer.h"
#include "GameObject.h"
#include "RenderCom.h"
#include "CamMove.h"
namespace yk
{
	// 管理场景内gameobject生命周期，渲染流程
	class SceneMgr : public Object
	{
	public:
		void init()
		{
			window.init();
		}

		void update(Timer& timer)
		{
			// window清理窗口
			auto res = window.prepare();

			if (not res)
			{
				window.terminate();
				Application::shutdown = true;
				return;
			}

			// 由gameobject上的组件去进行渲染
			GameObject::updateGameObjects();	// 后面加入不同周期，不同阶段更新gameobject

			// 刷新帧缓冲，事件
			window.finish();
			
		}

		void testscene()
		{
			auto gb = GameObject::createGameObject();
			gb->AddComponent<RenderCom>();
			gb->AddComponent<CamMove>();
		}
	private:
		Window window;
	};
}