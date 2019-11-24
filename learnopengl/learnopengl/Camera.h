#pragma once

#include "Component.h"

namespace yk
{
	class Camera : public Component
	{
		TYPE(yk::Camera, Component);

	public:
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

		void test()
		{
			auto p = transform()->position();

			p = transform()->View() * vec4(p, 1);

			cout << p.x << p.y << p.z << endl;
		}

	private:
		float m_fov = 45.0f;
		float m_near = 0.1f;
		float m_far = 1000.0f;

	};
}