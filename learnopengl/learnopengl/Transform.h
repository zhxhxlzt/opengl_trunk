#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#define TRANSFORM_DEBUG

namespace yk
{
	using namespace glm;
	class Transform : public Component
	{
		TYPE(yk::Transform, Component);

	private:
		// 世界空间
		vec3 m_position;
		vec3 m_scale;
		vec3 m_forward;
		vec3 m_right;
		vec3 m_up;
		qua<float> m_rotation;

		static const vec4 zero;
		static const vec4 sforward;
		static const vec4 sright;
		static const vec4 sup;

	public:
		Transform():
			Component(),
			m_position(vec3(.0f, .0f, .0f)),
			m_scale(vec3(1.0f, 1, 1)),
			m_forward(vec3(0, 0, -1.0f)), 
			m_right(vec3(1.0f, 0, 0)), 
			m_up(vec3(0, 1.0f, 0))
		{
			m_rotation = qua<float>(mat4(1));
		}
		
		void translate(vec3 moveVec)
		{
			m_position += moveVec;
		}

		void MoveTo(vec3 destPos)
		{
			m_position = destPos;
		}

		void rotate(float angle, vec3 axis)
		{
			angle = radians(angle);
			auto q = angleAxis(angle, axis);
			auto rot = q * m_rotation;
			setRotation(rot);
		}

		void setRotation(qua<float>& rot)
		{
			m_rotation = rot;
			mat4 mtx = mat4_cast(rot);
			m_forward = mtx * sforward;
			m_up = mtx * sup;
			m_right = mtx * sright;
		}

		void LookAt(vec3 targetPos)
		{
			auto mat = glm::lookAt(m_position, targetPos, m_up);
			m_forward = mat * vec4(m_forward, 1);
			m_right = mat * vec4(m_right, 1);
			m_up = mat * vec4(m_up, 1);
		}

		mat4 View()
		{
			return glm::lookAt(m_position, m_position + m_forward, m_up);
		}

		mat4 Model()
		{
			mat4 model(1);
			model = glm::translate(model, m_position);
			model = glm::scale(model, m_scale);
			model *= mat4_cast(m_rotation);
			return model;
		}

		vec3& position();

		vec3& scale();

		quat& rotation();

		vec3 forward();

		vec3 right();

		vec3 up();


		void debug()
		{
			cout << "-----------------transform-------------------" << endl;
			cout << "position:" << transform()->position() << endl;
			cout << "scale:   " << transform()->scale()  << endl;
			cout << "rotation:" << transform()->rotation() << endl;
		}

		~Transform()
		{
			cout << "tr d" << endl;
		}
	};
}