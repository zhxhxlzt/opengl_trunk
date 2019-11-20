#pragma once
#include <chrono>
#include <memory>
#include <iomanip>
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

#include "GraphicDebug.h"

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


inline ostream& operator<< (ostream& s, glm::vec3& v)
{
	s << v.x << " " << v.y << " " << v.z;
	return s;
}

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