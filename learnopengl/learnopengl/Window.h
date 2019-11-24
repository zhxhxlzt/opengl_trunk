#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Object.h"
#include "Timer.h"
#include "Input.h"


namespace yk
{
	class Window : public Object
	{
		TYPE(yk::Window, Object)
	public:
		void init()
		{
			glfwInit();
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
			if (window == NULL)
			{
				std::cout << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return;
			}
			
			glfwMakeContextCurrent(window);
			glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				std::cout << "Failed to initialize GLAD" << std::endl;
				return;
			}
			glEnable(GL_DEPTH_TEST);

			initCallback();
		}

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}

		static void processInput(GLFWwindow *window)
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}

		bool prepare()
		{
			if (not glfwWindowShouldClose(window))
			{
				processInput(window);

				glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				return true;
			}
			return false;
		}

		void finish()
		{
			glfwSwapBuffers(window);
			Input::process();
			glfwPollEvents();
		}

		void terminate()
		{
			glfwTerminate();
		}

		void SetCursorEnable(bool enabled)
		{
			if (enabled)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			else
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		}

	private:
		void initCallback()
		{
			glfwSetKeyCallback(window, Input::keyCallback);
			glfwSetMouseButtonCallback(window, Input::mouseButtonCallback);
			glfwSetScrollCallback(window, Input::scrollCallback);
			glfwSetCursorEnterCallback(window, cursorEnterCallback);
			glfwSetCursorPosCallback(window, cursorPositionCallback);
		}

		static void cursorEnterCallback(GLFWwindow* window, int event)
		{
			cursorEnter = event;
		}

		static void cursorPositionCallback(GLFWwindow* window, double x, double y)
		{
			if (cursorEnter == 1)
			{
				cursorEnter = -1;
				Input::initCursor(x, y);
			}

			Input::cursorPositionCallback(window, x, y);
		}

	private:
		GLFWwindow* window;
		static int cursorEnter;
	};
}