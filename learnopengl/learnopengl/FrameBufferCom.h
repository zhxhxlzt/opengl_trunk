#pragma once
#include "MonoBehaviour.h"
#include "GameObject.h"

namespace yk
{
	class FrameBufferCom : public MonoBehaviour
	{
		TYPE(yk::FrameBufferCom, MonoBehaviour);
	public:
		unsigned int fbo = -1;	// ֡����
		unsigned int tex;	// ��ɫ���帽��
		unsigned int dep;	// ��Ȼ��帽��
		unsigned int ste;	// ģ�建�帽��
		unsigned int rbo;	// ��Ⱦ�������

	public:
		virtual void Awake();
	};
}