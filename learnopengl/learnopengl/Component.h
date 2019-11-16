#pragma once
#include "Object.h"
namespace yk
{
	class GameObject;
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
			if (not m_gameObject.expired())
				return m_gameObject.lock();
			else
				return nullptr;
		}

	private:
		weak_ptr<GameObject> m_gameObject;
	};
}