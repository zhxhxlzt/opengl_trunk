#pragma once
#include "Object.h"
namespace yk
{
	class GameObject;
	class Transform;
	class Component : public Object
	{
		TYPE(yk::Component, yk::Object);

	public:
		void attachToGameObject(shared_ptr<GameObject> gb)
		{
			m_gameObject = gb;
		}

		shared_ptr<GameObject> gameObject()
		{
			return m_gameObject.lock();
		}

		shared_ptr<Transform> transform();

		template<typename T>
		shared_ptr<T> getComponent();

		template<typename T>
		shared_ptr<T> addComponent();

	private:
		weak_ptr<GameObject> m_gameObject;
	};
}