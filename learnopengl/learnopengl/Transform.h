﻿#pragma once
#include "Component.h"
#include <glm/glm.hpp>

#define TRANSFORM_DEBUG

namespace yk
{
	using namespace glm;
	class Transform : public Component
	{
		TYPE(yk::Transform, yk::Component);

	private:
		vec3 m_position;
		vec3 m_rotation;
		vec3 m_scale;
		vec3 m_forward;
		vec3 m_right;
		vec3 m_up;

	public:
		Transform():
			m_position(vec3(0, 0, 0)),
			m_rotation(vec3(0, 0, 0)),
			m_scale(vec3(1, 1, 1)),
			m_forward(vec3(0, 0, -1)), 
			m_right(vec3(1, 0, 0)), 
			m_up(vec3(0, 1, 0))
		{
		}

		void translate(vec3 moveVec)
		{
			m_position += moveVec;
		}

		void setPosition(vec3 destPos)
		{
			m_position = destPos;
		}

		void rotate(vec3 axis, float angle)
		{
			auto mat = glm::rotate(glm::mat4(1), angle, axis);
			m_forward = mat * vec4(m_forward, 1);
			m_right = mat * vec4(m_right, 1);
			m_up = mat * vec4(m_up, 1);
		}

		void lookAt(vec3 targetPos)
		{
			auto mat = glm::lookAt(m_position, targetPos, m_up);
			m_forward = mat * vec4(m_forward, 1);
			m_right = mat * vec4(m_right, 1);
			m_up = mat * vec4(m_up, 1);
		}
	
		mat4 transformMat()
		{
			/*trans = glm::translate(trans, vec3(0.5f, -0.5f, 0.0f));
			trans = glm::rotate(trans, (float)glfwGetTime(), vec3(0, 0, 1));*/

			auto trans = glm::lookAt(m_position, m_position + m_forward, m_up);
			trans = glm::scale(trans, m_scale);
			return trans;
		}

		vec3& position() { return m_position; }

		vec3 forward() { return m_forward; }

		vec3 right() { return m_right; }

		vec3 up() { return m_up; }
	};
}