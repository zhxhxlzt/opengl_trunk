
//#include "Transform.h"
#include "GameObject.h"

using namespace yk;

shared_ptr<Transform> Component::transform()
{
	return gameObject()->transform();
}

template<typename T>
shared_ptr<T> Component::getComponent()
{
	return gameObject()->GetComponent<T>();
}

template<typename T>
shared_ptr<T> Component::addComponent()
{
	return gameObject()->AddComponent<T>();
}
