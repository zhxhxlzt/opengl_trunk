#pragma once

#include "Component.h"

namespace yk
{
	class Camera : public Component
	{
		TYPE(yk::Camera, Component);

	public:
		Camera() :Component()
		{
			initFrameBuffer();
		}
		bool enablePostEffect = false;
		float fov() {}
		float far() {}
		float near() {}
		void SetFov(float fov) { m_fov = fov; }
		void SetNear(float near) { m_near = near; }
		void SetFar(float far) { m_far = far; }
		
		mat4 Proj()
		{
			return perspective(45.0f, 800.0f / 600, m_near, m_far);
		}

		unsigned int GetColorTex() { return tex; }

		void RenderPostEffect()
		{
			
		}

	private:
		float m_fov = 45.0f;
		float m_near = 0.1f;
		float m_far = 1000.0f;
		unsigned int fbo = -1;	// ֡����
		unsigned int tex;	// ��ɫ���帽��
		unsigned int dep;	// ��Ȼ��帽��
		unsigned int ste;	// ģ�建�帽��
		unsigned int rbo;	// ��Ⱦ�������

		void initFrameBuffer()
		{
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

			/*glGenTextures(1, &dep);
			glBindTexture(GL_TEXTURE_2D, dep);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dep, 0);*/

			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);


			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			{
				cout << "frame buffer complete" << endl;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	};
}