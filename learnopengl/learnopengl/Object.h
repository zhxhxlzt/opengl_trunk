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

// 使用此宏创建返回类型名的虚函数，用于基类指针类型匹配
#define TYPE(x) \
public: \
	virtual std::string GetTypeName() {return #x;}	\
private:



namespace yk
{
	class Object
	{
		TYPE(yk::Object);

	public:
		std::string ObjectName;


		~Object()
		{
			std::cout << "object deleted!" << std::endl;
		}
	};
}