#pragma once
#include "Component.h"
#include "Transform.h"

namespace yk
{
	enum class LightMode {DIR_LIGHT, POINT_LIGHT};
	class LightCom : public Component
	{
		TYPE(yk::LightCom, Component);

	public:
		LightMode lightMode() { return m_lightMode; }
		float& intensity() { return m_intensity; }
		glm::vec3& color() { return m_color; }
		vec3 dir() { return transform()->forward(); }

	private:
		LightMode m_lightMode = LightMode::DIR_LIGHT;
		float m_intensity = 1;
		glm::vec3 m_color = vec3(1, 1, 1);

	};
}