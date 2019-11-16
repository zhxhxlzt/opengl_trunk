
//#include "Transform.h"
#include "GameObject.h"

using namespace yk;

shared_ptr<Transform> Component::transform()
{
	return gameObject()->transform();
}

