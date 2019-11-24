#include "Transform.h"

using namespace glm;
using namespace yk;

const vec4 Transform::zero = vec4(0, 0, 0, 1);
const vec4 Transform::sforward = vec4(0, 0, -1, 1);
const vec4 Transform::sright = vec4(1, 0, 0, 1);
const vec4 Transform::sup = vec4(0, 1, 0, 1);

vec3& Transform::position() { return m_position; }

vec3& Transform::scale() { return m_scale; }

vec3 Transform::forward() { return m_forward; }

vec3 Transform::right() { return m_right; }

vec3 Transform::up() { return m_up; }

quat& Transform::rotation()
{
	return m_rotation;
}