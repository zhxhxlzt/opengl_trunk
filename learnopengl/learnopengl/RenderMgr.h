#pragma once

#include "Object.h"
#include "Window.h"
#include "SceneMgr.h"
#include "Application.h"
#include "Renderer.h"
#include "Timer.h"
#include "FrameBufferCom.h"
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
			glCheckError();
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

			// 脚本逻辑后面从渲染中移除
			for (auto &p : SceneMgr::currentScene()->GameObjects())
			{
				p->UpdateComponent();
			}

			// 设置目标帧缓冲
			auto postEffect = SceneMgr::currentScene()->postEffectObject();
			if (postEffect)
			{
				auto fbCom = postEffect->GetComponent<FrameBufferCom>();
				glBindFramebuffer(GL_FRAMEBUFFER, fbCom->fbo);
			}

			// 由gameobject上的组件去进行渲染
			// 后面加入不同周期，不同阶段更新gameobject
			for (auto &p : SceneMgr::currentScene()->GameObjects())
			{
				Rendering(p);
			}

			// 渲染天空盒
			auto p = SceneMgr::currentScene()->cubeMapObject();
			if (p)
			{
				glDepthFunc(GL_LEQUAL);
				Rendering(p);
				glDepthFunc(GL_LESS);
			}

			if (postEffect)
			{
				PostRendering(postEffect);
			}
			
			m_window->finish();
		}
		static shared_ptr<Window> window() { return m_window; }
	private:
		static void Rendering(shared_ptr<GameObject> p)
		{
			auto render = p->GetComponent<Renderer>();
			if (render)
			{
				// 渲染前调用
				for (auto &com_ptr : p->GetComponents<Component>())
					com_ptr->OnRendering();

				render->Rendering();

				// 渲染后调用
				for (auto &com_ptr : p->GetComponents<MonoBehaviour>())
					com_ptr->LateUpdate();
			}
		}
		static void PostRenderPrepare(shared_ptr<GameObject> p)
		{
			auto fbCom = p->GetComponent<FrameBufferCom>();
			glBindFramebuffer(GL_FRAMEBUFFER, fbCom->fbo);
		}
		
		static void PostRendering(shared_ptr<GameObject> p)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			Rendering(p);
			glEnable(GL_DEPTH_TEST);
			auto fboCom = p->GetComponent<FrameBufferCom>();
			glBindFramebuffer(GL_FRAMEBUFFER, fboCom->rbo);
		}
		static shared_ptr<Window> m_window;
	};
}