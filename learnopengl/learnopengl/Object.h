#pragma once
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <type_traits>
#include <exception>

#define not !
#define or  ||
#define and &&


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

		~Object()
		{
			std::cout << "object deleted!" << std::endl;
		}
	};
}