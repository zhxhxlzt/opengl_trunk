#pragma once
#include <chrono>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <iostream>
#include <type_traits>
#include <exception>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

#define not !
#define or  ||
#define and &&

using namespace std;

#define TYPE(x, y) \
private: \
	friend class GameObject; \
	static constexpr char type_name[] = #x; \
	virtual std::string GetTypeName() { return #x; } \
	using ParentType = y;


namespace yk
{
	class Object
	{
		TYPE(yk::Object, yk::Object);

	protected:
		bool m_alive = true;

	public:
		virtual void Destroy()
		{
			m_alive = false;
		}

		operator bool()
		{
			return m_alive;
		}
	};
}