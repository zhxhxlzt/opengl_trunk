#pragma once

#include "Object.h"
#include "Window.h"
#include "Application.h"
#include "Renderer.h"
#include "Timer.h"
#include "FrameBufferCom.h"
#include "Scene.h"
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
		static void RenderUpdate(shared_ptr<Scene> currentScene)
		{
			/*
			1. ����׼��
			2. gameobject�������
			3. ���³������ݣ�
				1. view��projection��uniform buffer
				2. �ƹ�����
			4. ��Ⱦ��ģ�ͣ���պУ�
			5. ����
			6. swapbuffers, ����ui�¼�
			*/


			auto res = m_window->prepare();
			if (not res)
			{
				m_window->terminate();
				Application::shutdown = true;
				return;
			}

			// ����Ŀ��֡����
			auto postEffect = currentScene->postEffectObject();
			if (postEffect)
			{
				auto fbCom = postEffect->GetComponent<FrameBufferCom>();
				glBindFramebuffer(GL_FRAMEBUFFER, fbCom->fbo);
			}

			// ��gameobject�ϵ����ȥ������Ⱦ
			// ������벻ͬ���ڣ���ͬ�׶θ���gameobject
			for (auto &p : currentScene->GameObjects())
			{
				Rendering(p);
			}

			// ��Ⱦ��պ�
			auto p = currentScene->cubeMapObject();
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
				// ��Ⱦǰ����
				for (auto &com_ptr : p->GetComponents<Component>())
					com_ptr->OnRendering();

				render->Rendering();

				// ��Ⱦ�����
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