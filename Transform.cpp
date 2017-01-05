#include "Transform.h"

Transform::Transform(Vec3 translation, Vec3 rotation, Vec3 scale) : m_translation(translation), m_rotation(rotation), m_scale(scale)
{
}

Transform::~Transform()
{
}
