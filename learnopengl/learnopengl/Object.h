#pragma once
#include <chrono>
#include <memory>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <map>
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
#define STB_IMAGE_IMPLEMENTATION
#define not !
#define or  ||
#define and &&

using namespace std;

#define toString(x) #x

#define TYPE(x, y) \
public: \
	static constexpr char type_name[] = #x; \
	virtual std::string GetTypeName() { return #x; } \
	using ParentType = y;	\
	//using y::y;

#define OBJECT(x) \
protected: \
	static constexpr char type_name[] = #x; \
	virtual std::string GetTypeName() { return #x; } \
	using ParentType = x;	\


inline ostream& operator<< (ostream& s, glm::vec3 v)
{
	s << v.x << ", " << v.y << "," << v.z;
	return s;
}

inline ostream& operator<< (ostream& s, glm::vec4 v)
{
	s << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
	return s;
}

inline ostream& operator<< (ostream& s, glm::quat v)
{
	s << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
	return s;
}

inline ostream& operator<< (ostream& s, glm::mat4 m)
{
	s << "[[" << m[0] << "][" << m[1] << "][" << m[2] << "][" << m[3] << "]]" << endl;
	return s;
}

namespace yk
{
	class Object
	{
		OBJECT(yk::Object);
	protected:
		bool m_alive = true;

	public:
		Object()
		{
			m_id = GenID();
		}

		template<typename B, typename D>
		static constexpr bool IsBaseOf()
		{
			if (D::type_name != D::ParentType::type_name)
			{
				if (B::type_name == D::type_name)
					return true;
				return IsBaseOf<B, D::ParentType>();
			}
			return false;
		}

		virtual void Destroy()
		{
			m_alive = false;
		}

		operator bool()
		{
			return m_alive;
		}

		unsigned int GetInstanceID()
		{
			return m_id;
		}

	private:
		unsigned int m_id;
		static unsigned int distribute_id;
		unsigned int GenID()
		{
			distribute_id += 1;
			return distribute_id;
		}
	};
}