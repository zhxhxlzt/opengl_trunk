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

		/*template<typename T>
		shared_ptr<T> getComponent()
		{
			auto gb = gameObject()
			return gb->;
		}

		template<typename T>
		shared_ptr<T> addComponent()
		{
			return (gameObject())->AddComponent<T>();
		}*/


	private:
		weak_ptr<GameObject> m_gameObject;
	};
}