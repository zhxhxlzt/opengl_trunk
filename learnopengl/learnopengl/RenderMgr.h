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
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				auto tex = postEffect->GetComponent<FrameBufferCom>()->tex;
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, tex);
				

				unsigned int vao, vbo;
				vector<float> vertices = {
					0, 0, 0, 0, 0,
					1, 0, 0, 1, 0,
					1, 1, 0, 1, 1
				};
				glGenVertexArrays(1, &vao);
				glGenBuffers(1, &vbo);
				
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);

				Shader shd = Shader("frametest.vert", "frametest.frag");
				shd.use();
				shd.set("ftex", 0);
				glDrawArrays(GL_TRIANGLES, 0, 3);

				/*glDisable(GL_DEPTH_TEST);
				Rendering(postEffect);
				glEnable(GL_DEPTH_TEST);*/
			}

			// ---------------- 测试帧缓冲对象是否正常写入颜色缓冲

			//auto tex = postEffect->GetComponent<FrameBufferCom>()->tex;
			//float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			//	// positions   // texCoords
			//	-1.0f,  1.0f,  0.0f, 1.0f,
			//	-1.0f, -1.0f,  0.0f, 0.0f,
			//	 1.0f, -1.0f,  1.0f, 0.0f,

			//	-1.0f,  1.0f,  0.0f, 1.0f,
			//	 1.0f, -1.0f,  1.0f, 0.0f,
			//	 1.0f,  1.0f,  1.0f, 1.0f
			//};
			//unsigned int quadVAO, quadVBO;
			//glGenVertexArrays(1, &quadVAO);
			//glGenBuffers(1, &quadVBO);
			//glBindVertexArray(quadVAO);
			//glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			//glEnableVertexAttribArray(0);
			//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			//glEnableVertexAttribArray(1);
			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

			//Shader shader("5.1.framebuffers.vs", "5.1.framebuffers.fs");
			//shader.use();
			//shader.set("texture_diffuse1", 0);

			//glBindVertexArray(quadVAO);
			//glBindTexture(GL_TEXTURE_2D, tex);	// use the color attachment texture as the texture of the quad plane
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			// 刷新帧缓冲，事件
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
			
			glDisable(GL_DEPTH_TEST);
			//glDepthMask(GL_FALSE);
			
			Rendering(p);
			glEnable(GL_DEPTH_TEST);
			//glDepthMask(GL_TRUE);
		}
		static shared_ptr<Window> m_window;
	};
}