#pragma once

#include "Object.h"
#include "Window.h"
#include "SceneMgr.h"
#include "Application.h"
#include "Renderer.h"
#include "Timer.h"

namespace yk
{
	class RenderMgr : public Object
	{
		TYPE(yk::RenderMgr, Object);

	public:
		static void InitWindow()
		{
			m_window = make_shared<Window>();
			m_window->init();
		}

		static void RenderUpdate(Timer &timer)
		{
			auto res = m_window->prepare();
			if (not res)
			{
				m_window->terminate();
				Application::shutdown = true;
				return;
			}

			for (auto &p : SceneMgr::currentScene()->GameObjects())
			{
				p->UpdateComponent();
			}

			// ��gameobject�ϵ����ȥ������Ⱦ
			// ������벻ͬ���ڣ���ͬ�׶θ���gameobject
			for (auto &p : SceneMgr::currentScene()->GameObjects())
			{
				auto render = p->GetComponent<Renderer>();
				if (render)
				{
					// ��Ⱦǰ����
					for (auto &com_ptr : p->GetComponents<Component>())
						com_ptr->OnRendering();

					render->Rendering();

					// ��Ⱦ�����
					for (auto &com_ptr : p->GetComponents<MonoBehaviour>())
						com_ptr->LateUpdate();
				}
			}

			// ˢ��֡���壬�¼�
			m_window->finish();
		}
		static shared_ptr<Window> window() { return m_window; }
	private:
		static shared_ptr<Window> m_window;
	};
}