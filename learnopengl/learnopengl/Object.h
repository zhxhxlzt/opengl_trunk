#pragma once
#include <chrono>
#include <memory>
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