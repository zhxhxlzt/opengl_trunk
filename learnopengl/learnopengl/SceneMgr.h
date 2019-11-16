#pragma once
#include "Object.h"
#include "Window.h"
#include "Timer.h"
#include "GameObject.h"
#include "RenderCom.h"

namespace yk
{
	// ��������gameobject�������ڣ���Ⱦ����
	class SceneMgr : public Object
	{
	public:
		void init()
		{
			window.init();
		}

		void update(Timer& timer)
		{
			// window������
			auto res = window.prepare();

			if (not res)
			{
				window.terminate();
				Application::shutdown = true;
				return;
			}

			// ��gameobject�ϵ����ȥ������Ⱦ
			GameObject::updateGameObjects();	// ������벻ͬ���ڣ���ͬ�׶θ���gameobject

			// ˢ��֡���壬�¼�
			window.finish();
		}

		void testscene()
		{
			auto gb = GameObject::createGameObject();
			gb->AddComponent<RenderCom>();
		}
	private:
		Window window;
	};
}