#pragma once

#include "MonoBehaviour.h"
#include "Input.h"

namespace yk
{
	class CamMove : public MonoBehaviour
	{
		TYPE(yk::CamMove, yk::MonoBehaviour);

	public:
		virtual void Update()
		{
			if (Input::getkey(GLFW_KEY_A) || Input::getkeydown(GLFW_KEY_A))
			{
				auto delta = 0.1f * transform()->right();
				transform()->position() -= 0.1f * transform()->right();
			}

			if (Input::getkey(GLFW_KEY_D) || Input::getkeydown(GLFW_KEY_D))
			{
				auto delta = 0.1f * transform()->right();
				transform()->position() += delta;
			}

			if (Input::getkey(GLFW_KEY_W) || Input::getkeydown(GLFW_KEY_W))
			{
				transform()->position() += 0.1f * transform()->forward();
			}

			if (Input::getkey(GLFW_KEY_S) || Input::getkeydown(GLFW_KEY_S))
			{
				transform()->position() -= 0.1f * transform()->forward();
			}

			if (Input::getkey(GLFW_KEY_Q) || Input::getkeydown(GLFW_KEY_Q))
			{
				transform()->position() -= 0.1f * transform()->up();
			}

			if (Input::getkey(GLFW_KEY_E) || Input::getkeydown(GLFW_KEY_E))
			{
				transform()->position() += 0.1f * transform()->up();
			}

			if (Input::getkeydown(GLFW_MOUSE_BUTTON_1))
			{
				cout << "mouse pressed!" << endl;
			}

			if (Input::getkeyup(GLFW_MOUSE_BUTTON_1))
			{
				cout << "mouse released!" << endl;
			}
		}
	};
}