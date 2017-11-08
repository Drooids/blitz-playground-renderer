#include "Vector2D.h"

Vector2D::Vector2D() : m_x(0), m_y(0) { }
Vector2D::Vector2D(float x, float y) : m_x(x), m_y(y) {}

Vector2D::~Vector2D() {}

const float Vector2D::getX() { return m_x; }
const float Vector2D::getY() { return m_y; }

void Vector2D::setX(float x) { m_x = x; }
void Vector2D::setY(float y) { m_y = y; }

float Vector2D::length() { return sqrtf(m_x * m_x + m_y * m_y); }

void Vector2D::normalize() {
	float L = length();
	if (L > 0)
	{
		(*this) *= 1.0f / L;
	}
}
