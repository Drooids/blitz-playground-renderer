#ifndef VECTOR3D_H_DEFINED
#define VECTOR3D_H_DEFINED

#include <iostream>
#include <math.h>

class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y, float z);

	virtual ~Vector3D();

	virtual const float getX();
	virtual const float getY();
	virtual const float getZ();

	virtual void setX(float x);
	virtual void setY(float y);
	virtual void setZ(float y);

	virtual float length();

	virtual void normalize();
	virtual Vector3D cross(const Vector3D& v1) const;
	virtual float distance(const Vector3D& v) const;

	Vector3D operator+(const Vector3D& v2) const {
		return Vector3D(m_x + v2.m_x, m_y + v2.m_y, m_z + v2.m_z);
	}

	friend Vector3D& operator+=(Vector3D& v1, const Vector3D& v2) {
		v1.m_x += v2.m_x;
		v1.m_y += v2.m_y;
		v1.m_z += v2.m_z;

		return v1;
	}

	Vector3D operator-(const Vector3D& v2) const {
		return Vector3D(m_x - v2.m_x, m_y - v2.m_y, m_z - v2.m_z);
	}

	friend Vector3D& operator-=(Vector3D& v1, const Vector3D& v2) {
		v1.m_x -= v2.m_x;
		v1.m_y -= v2.m_y;
		v1.m_z -= v2.m_z;

		return v1;
	}

	Vector3D operator*(float scalar) {
		return Vector3D(m_x * scalar, m_y * scalar, m_z * scalar);
	}

	Vector3D& operator*=(float scalar) {
		m_x *= scalar;
		m_y *= scalar;
		m_z *= scalar;

		return *this;
	}

	Vector3D operator/(float scalar) {
		return Vector3D(m_x / scalar, m_y / scalar, m_z / scalar);
	}

	Vector3D& operator/=(float scalar) {
		m_x /= scalar;
		m_y /= scalar;
		m_z /= scalar;

		return *this;
	}

	friend std::ostream &operator<<(std::ostream &os, Vector3D* const &v) {
		return os << "vector: \n"
			<< "\tx: " << v->getX() << "\n"
			<< "\ty: " << v->getY()
			<< "\tz: " << v->getZ();
	}

private:

	float m_x;
	float m_y;
	float m_z;
};

#endif /* defined(VECTOR3D_H_DEFINED) */
