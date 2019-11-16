#pragma once
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Behaviour.h"
#include "Timer.h"

namespace yk
{
	class GameObject : public Object
	{
		TYPE(yk::GameObject, yk::Object);

	public:
		template<typename T>
		std::shared_ptr<T> AddComponent()
		{
			if (not std::is_base_of_v<Component, T>)
			{
				throw new std::exception("类型错误，不是Component的子类！");
			}

			if (is_same_v<Transform, T>)
			{
				throw new exception("不允许添加Transform");
			}

			return _AddComponent<T>(is_base_of<Behaviour, T>());
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

		void Destroy()
		{
			m_alive = false;
			for (auto& com : m_behaviours)
			{
				com->OnDestroy();
			}
			m_comps.clear();
			m_behaviours.clear();
		}

		void Destroy(shared_ptr<Component> com)
		{
			if (com->GetTypeName() == Transform::type_name)
			{
				throw new exception("不允许删除Transform");
			}
			auto res = find(m_comps.begin(), m_comps.end(), com);
			if (res != m_comps.end())
			{
				m_comps.erase(res);
			}
		}

		void Destroy(shared_ptr<Behaviour> com)
		{
			auto res = find(m_behaviours.begin(), m_behaviours.end(), com);
			if (res != m_behaviours.end())
			{
				m_behaviours.erase(res);
				com->OnDestroy();
				com->Destroy();
			}
		}

	private:
		template<typename T>
		shared_ptr<T> _AddComponent(std::true_type)
		{
			cout << "behaviour comp" << endl;
			std::shared_ptr<T> com_ptr = std::make_shared<T>();
			com_ptr->attachToGameObject(m_this);
			this->m_comps.push_back(com_ptr);
			com_ptr->Awake();
			m_behaviours.push_back(com_ptr);
			return com_ptr;
		}
		template<typename T>
		shared_ptr<T> _AddComponent(std::false_type)
		{
			std::shared_ptr<T> com_ptr = std::make_shared<T>();
			com_ptr->attachToGameObject(m_this);
			this->m_comps.push_back(com_ptr);
			return com_ptr;
		}
		virtual void UpdateComponent()
		{
			for (auto& com : m_behaviours)
			{
				com->Update();
			}
		}

	private:
		std::vector<std::shared_ptr<Component>> m_comps;
		vector<shared_ptr<Behaviour>> m_behaviours;
		shared_ptr<Transform> m_transform;
		shared_ptr<GameObject> m_this;

		// 全局管理，先放这里方便使用，后面做修改
	public:
		static shared_ptr<GameObject> createGameObject()
		{
			GameObject* gb_ptr = new GameObject;
			shared_ptr<GameObject> gb(gb_ptr);
			gb->m_this = gb;

			auto tr = make_shared<Transform>();
			tr->attachToGameObject(gb);

			gb->m_comps.push_back(tr);
			gb->m_transform = tr;
			GameObject::s_gameObjects.push_back(gb);
			return gb;
		}


		static void updateGameObjects()
		{
			for (auto& gb : s_gameObjects)
			{
				gb->UpdateComponent();
			}
		}

	private:
		static vector<shared_ptr<GameObject>> s_gameObjects;
	};

	vector<shared_ptr<GameObject>> GameObject::s_gameObjects;

}