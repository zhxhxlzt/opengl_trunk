#pragma once
#include "Object.h"
#include "Component.h"
#include "Transform.h"
#include "Behaviour.h"
#include "Timer.h"
#include "Camera.h"
#include "MonoBehaviour.h"
#include "ModelFilter.h"
#include "ModelRenderer.h"



namespace yk
{
	class GameObject : public Object
	{
		TYPE(yk::GameObject, Object);
	public:
		GameObject() :Object()
		{
			// todo 这里执行会造成在SceneMgr中创建GameObject时出现没有提示的异常
			//m_this = shared_ptr<GameObject>(this);
			//m_transform = make_shared<Transform>();
			//m_transform->attachToGameObject(m_this);
		}
		std::shared_ptr<Transform> transform() { return m_transform; }

		template<typename T>
		shared_ptr<T> AddComponent()
		{
			if (not std::is_base_of_v<Component, T>)
			{
				throw new std::exception("类型错误，不是Component的子类！");
			}

			if (is_same_v<Transform, T>)
			{
				throw new exception("不允许添加Transform");
			}

			return _AddComponent<T>(is_base_of<MonoBehaviour, T>());
		}

		template<typename T>
		shared_ptr<T> GetComponent()
		{
			if (not std::is_base_of<Component, T>().value)
			{
				throw new std::exception("类型错误，不是Component的子类！");
			}
			std::string type_name = T::type_name;
			std::shared_ptr<T> target;
			for (auto &com_ptr : this->m_comps)
			{
				// 使用这个每次耗时约2600纳秒
				/*if (com_ptr->GetTypeName() == type_name)
				{
					target = std::static_pointer_cast<T>(com_ptr);
					break;
				}*/

				// 使用这个每次耗时约770纳秒，且可判断能否转成基类
				target = dynamic_pointer_cast<T>(com_ptr);
				if (target)
					break;
			}
			return target;
		}

		template<>
		shared_ptr<Transform> AddComponent()
		{
			return m_transform;
		}

		template<>
		shared_ptr<Transform> GetComponent()
		{
			return m_transform;
		}

		template<typename T> 
		vector<shared_ptr<T>> GetComponents()
		{
			vector<shared_ptr<T>> tarComps;
			shared_ptr<T> target;
			for (auto &com_ptr : m_comps)
			{
				target = dynamic_pointer_cast<T>(com_ptr);
				if (target)
					tarComps.push_back(target);
			}
			return tarComps;
		}

		void Destroy()
		{
			m_this.reset();
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

		void Destroy(shared_ptr<MonoBehaviour> com)
		{
			auto res = find(m_behaviours.begin(), m_behaviours.end(), com);
			if (res != m_behaviours.end())
			{
				m_behaviours.erase(res);
				com->OnDestroy();
				com->Destroy();
			}
		}

		virtual void UpdateComponent()
		{
			for (auto& com : m_behaviours)
			{
				com->Update();
			}
		}
	private:
		template<typename T>
		shared_ptr<T> _AddComponent(std::true_type)
		{
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

	private:
		std::vector<std::shared_ptr<Component>>		m_comps;
		vector<shared_ptr<MonoBehaviour>>				m_behaviours;
		shared_ptr<Transform>						m_transform;
		shared_ptr<GameObject>						m_this;

	// STATIC
	public:
		static shared_ptr<GameObject> Create()
		{
			shared_ptr<GameObject> gb = make_shared<GameObject>();
			gb->m_this = gb;
			gb->m_transform = make_shared<Transform>();
			gb->m_transform->attachToGameObject(gb);
			return gb;
		}
	};
}