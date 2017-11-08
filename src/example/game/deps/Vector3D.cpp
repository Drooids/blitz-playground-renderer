#include <Math.h>
#include "Vector3D.h"

Vector3D::Vector3D() : m_x(0), m_y(0), m_z(0) { }
Vector3D::Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {}

Vector3D::~Vector3D() {}

const float Vector3D::getX() { return m_x; }
const float Vector3D::getY() { return m_y; }
const float Vector3D::getZ() { return m_z; }

void Vector3D::setX(float x) { m_x = x; }
void Vector3D::setY(float y) { m_y = y; }
void Vector3D::setZ(float z) { m_z = z; }

float Vector3D::length() { return sqrtf(m_x * m_x + m_y * m_y + m_z * m_z); }

void Vector3D::normalize() {
	float L = length();
	if (L > 0)
	{
		(*this) *= 1.0f / L;
	}
}

Vector3D Vector3D::cross(const Vector3D& v1) const {
	return Vector3D(m_y*v1.m_z - m_z*v1.m_y, m_z*v1.m_x - m_x*v1.m_z, m_x*v1.m_y - m_y*v1.m_x);
}

float Vector3D::distance(const Vector3D& v) const {
	return sqrtf( (v.m_x - m_x) * (v.m_x - m_x) + (v.m_y - m_y) * (v.m_y - m_y) + (v.m_z - m_z) * (v.m_z - m_z) );
}
