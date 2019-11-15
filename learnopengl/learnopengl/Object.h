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

#define TYPE(x) \
private: \
	friend class GameObject; \
	static constexpr char type_name[] = #x; \
	virtual std::string GetTypeName() { return #x; } \


namespace yk
{
	class Object
	{
		TYPE(yk::Object);
	};
}