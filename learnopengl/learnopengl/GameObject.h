#pragma once
#include "Object.h"
#include "Component.h"

namespace yk
{
	class GameObject : public Object
	{
		TYPE(yk::GameObject);

	public:
		template<typename T>
		std::shared_ptr<T> AddComponent()
		{
			if (not std::is_base_of<Component, T>().value)
			{
				throw new std::exception("类型错误，不是Component的子类！");
			}
			std::shared_ptr<T> com_ptr = std::make_shared<T>();
			this->m_comps.push_back(com_ptr);
			return com_ptr;
		}

		template<typename T>
		std::shared_ptr<T> GetComponent()
		{
			if (not std::is_base_of<Component, T>().value)
			{
				throw new std::exception("类型错误，不是Component的子类！");
			}
			std::string type_name = T::type_name;
			std::shared_ptr<T> target;
			for (auto &com_ptr : this->m_comps)
			{
				if (com_ptr->GetTypeName() == type_name)
				{
					target = std::static_pointer_cast<T>(com_ptr);
					break;
				}
			}
			return target;
		}


	private:
		std::vector<std::shared_ptr<Component>> m_comps;
	};
}