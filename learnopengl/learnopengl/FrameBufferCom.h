#include "MonoBehaviour.h"
#include "GameObject.h"

namespace yk
{
	class FrameBufferCom : public MonoBehaviour
	{
		TYPE(yk::FrameBuffCom, MonoBehaviour);
	public:
		unsigned int fbo;	// Ö¡»º³å
		unsigned int tex;	// ÑÕÉ«»º³å¸½Ìå
		unsigned int dep;	// Éî¶È»º³å¸½¼þ
		unsigned int ste;	// Ä£°å»º³å¸½¼þ
		unsigned int rbo;	// äÖÈ¾»º³å¶ÔÏó

	public:
		virtual void Awake()
		{
			glGenBuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		virtual void OnRendering()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		virtual void LateUpdate()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, rbo);
		}
	};
}