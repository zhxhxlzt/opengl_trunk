#include "MonoBehaviour.h"
#include "GameObject.h"

namespace yk
{
	class FrameBufferCom : public MonoBehaviour
	{
		TYPE(yk::FrameBuffCom, MonoBehaviour);
	public:
		unsigned int fbo;	// ֡����
		unsigned int tex;	// ��ɫ���帽��
		unsigned int dep;	// ��Ȼ��帽��
		unsigned int ste;	// ģ�建�帽��
		unsigned int rbo;	// ��Ⱦ�������

	public:
		virtual void Awake();

		virtual void OnRendering();

		virtual void LateUpdate();
	};
}