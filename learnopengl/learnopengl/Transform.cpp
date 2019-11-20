#include "Transform.h"

using namespace glm;
using namespace yk;

const vec4 Transform::m_basePosition = vec4(0, 0, 0, 1);
const vec4 Transform::m_baseForward = vec4(0, 0, -1, 1);
const vec4 Transform::m_baseRight = vec4(1, 0, 0, 1);
const vec4 Transform::m_baseUp = vec4(0, 1, 0, 1);
const vec4 Transform::m_baseScale = vec4(1, 1, 1, 1);
const mat4 Transform::m_baseRotation = mat4(1);