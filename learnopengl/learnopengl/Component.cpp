#include "Transform.h"
#include "GameObject.h"

using namespace yk;

void Component::attachToGameObject(shared_ptr<GameObject> gb)
{
	m_gameObject = gb;
	m_transform = gb->transform();
}

// ERROR: ����ͨ������������������gameObject����ͬ����
template<typename T>
shared_ptr<T> Component::GetComponent()
{
	return gameObject()->GetComponent<T>();
}

template<typename T>
shared_ptr<T> Component::AddComponent()
{
	return gameObject()->AddComponent<T>();
}

