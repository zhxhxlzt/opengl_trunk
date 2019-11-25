#pragma once
#include "Object.h"
namespace yk
{
	class GameObject;
	class Transform;
	class Component : public Object
	{
		TYPE(yk::Component, Object);

	public:
		Component() : Object(){}
		Component(weak_ptr<GameObject> gb) : Object()
		{
			m_gameObject = gb;
		}

		void attachToGameObject(shared_ptr<GameObject> gb);

		shared_ptr<GameObject> gameObject()
		{
			return m_gameObject.lock();
		}

		shared_ptr<Transform> transform()
		{
			if (m_transform.expired())
			{
				cout << "transform expired" << endl;
			}
			return m_transform.lock();
		}

		// 在此物体渲染前调用
		virtual void OnRendering() {}

		template<typename T>
		inline shared_ptr<T> GetComponent();

		template<typename T>
		inline shared_ptr<T> AddComponent();

	private:
		template<typename T>
		friend class ComponentMgr;
		weak_ptr<GameObject> m_gameObject;
		weak_ptr<Transform> m_transform;
	};
}