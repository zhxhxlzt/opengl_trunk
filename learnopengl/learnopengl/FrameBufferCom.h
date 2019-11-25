#include "MonoBehaviour.h"
#include "GameObject.h"

namespace yk
{
	class FrameBufferCom : public MonoBehaviour
	{
		TYPE(yk::FrameBuffCom, MonoBehaviour);
	public:
		unsigned int fbo;	// 帧缓冲
		unsigned int tex;	// 颜色缓冲附体
		unsigned int dep;	// 深度缓冲附件
		unsigned int ste;	// 模板缓冲附件
		unsigned int rbo;	// 渲染缓冲对象

	public:
		virtual void Awake();

		virtual void OnRendering();

		virtual void LateUpdate();
	};
}