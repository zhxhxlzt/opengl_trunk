#pragma once
#include "Object.h"

namespace yk
{
	class Input : public Object
	{
		TYPE(yk::Input, Object);
	public:
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			//cout << "key: " << key << "scancode: " << scancode << "action: " << action << "mods: " << mods << endl;
			actions[key] = action;
		}

		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
		{
			//cout << "button: " << button << "action: " << action << "mods: " << mods << endl;
			actions[button] = action;
		}

		static void cursorPositionCallback(GLFWwindow* window, double x, double y)
		{
			////cout << "mouseX: " << x << "mouseY: " << y << endl;
			deltaX += (Input::x - x);
			deltaY += (Input::y - y);

			Input::x = x;
			Input::y = y;

			//cout << "deltaX: " << deltaX << " deltaY: " << deltaY << endl;
		}
		static void scrollCallback(GLFWwindow* window, double x, double y)
		{
			//cout << "scrollX: " << x << "scrollY: " << y << endl;
			scroll += y;
		}

		static void initCursor(double x, double y)
		{
			Input::x = x;
			Input::y = y;
			Input::deltaX = 0;
			Input::deltaY = 0;
		}

		static void process()
		{
			deltaX = 0;
			deltaY = 0;
			deltaScroll = 0;

			for (auto &e : actions)
			{
				if (e == 1) e = 2;	// 下一帧由按下状态切换到按住状态
				else if (e == 0) e = -1;	// 下一帧由松开状态切换到无状态
			}
		}

		static bool getkeydown(int key)
		{
			return actions[key] == GLFW_PRESS;
		}

		static bool getkeyup(int key)
		{
			return actions[key] == GLFW_RELEASE;
		}

		static bool getkey(int key)
		{
			return actions[key] == GLFW_REPEAT;
		}

		static double getCursorDeltaX() { return deltaX; }

		static double getCursorDeltaY() { return deltaY; }

		static double getCursorPosX() { return x; }

		static double getCursorPosY() { return y; }

	private:
		static vector<int> actions;
		static double x;
		static double y;
		static double deltaX;
		static double deltaY;
		static double scroll;
		static double deltaScroll;
	};
}