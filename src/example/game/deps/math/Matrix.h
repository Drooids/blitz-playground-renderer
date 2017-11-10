#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace Math
{
	namespace Matrix
	{
		inline glm::mat3 rotation(const float degrees, const glm::vec3& axis)
		{
			float cos = glm::cos(glm::radians(degrees));
			float sin = glm::sin(glm::radians(degrees));

			float c = 1.0f - cos;

			glm::mat3 I(1.0f);

			float x = axis.x;
			float y = axis.y;
			float z = axis.z;

			float xx = x * x;
			float yy = y * y;
			float zz = z * z;

			float xy = x * y;
			float xz = x * z;

			float yz = y * z;

			glm::mat3 AM(
				xx, xy, xz,
				xy, yy, yz,
				xz, yz, zz
				);

			glm::mat3 RM(
				0, -z, y,
				z, 0, -x,
				-y, x, 0
				);

			I = cos * I;
			AM = c * AM;
			RM = sin * RM;

			return glm::mat3(I + AM + RM);
		}
	}
}