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

// ʹ�ô˺괴���������������麯�������ڻ���ָ������ƥ��
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