/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2020
* \note			Course: GAM200
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "elsHeader.h"
#include "elsMath.h"
#include "elsMatrix4.h"


namespace els
{
	template <typename T>
	class Transform3
	{
	public:
		template <typename S>
		static inline constexpr Matrix4<T> translate(S x, S y, S z)
		{
			return Matrix4<T>{
				1, 0, 0, T(x),
					0, 1, 0, T(y),
					0, 0, 1, T(z),
					0, 0, 0, 1,

			};
		}
		template <typename S>
		static inline constexpr Matrix4<T> translate(const Vector3<S>& v)
		{
			return translate(v.x, v.y, v.z);
		}

		template <typename S>
		static inline constexpr Matrix4<T> rotateX(S angle)
		{
			const T c = cos(static_cast<T>(angle));
			const T s = sin(static_cast<T>(angle));

			return Matrix4<T>{
				1, 0, 0, 0,
					0, c, -s, 0,
					0, s, c, 0,
					0, 0, 0, 1
			};
		}
		template <typename S>
		static inline constexpr Matrix4<T> rotateY(S angle)
		{
			const T c = cos(static_cast<T>(angle));
			const T s = sin(static_cast<T>(angle));

			return Matrix4<T>{
				c, 0, s, 0,
					0, 1, 0, 0,
					-s, 0, c, 0,
					0, 0, 0, 1
			};
		}
		template <typename S>
		static inline constexpr Matrix4<T> rotateZ(S angle)
		{
			const T c = cos(static_cast<T>(angle));
			const T s = sin(static_cast<T>(angle));

			return Matrix4<T>{
				c, -s, 0, 0,
					s, c, 0, 0,
					0, 0, 1, 0,
					0, 0, 0, 1
			};
		}

		template <typename S>
		static inline constexpr Matrix4<T> scale(S x, S y, S z)
		{
			return Matrix4<T>{
				static_cast<T>(x), 0, 0, 0,
					0, static_cast<T>(y), 0, 0,
					0, 0, static_cast<T>(z), 0,
					0, 0, 0, 1,
			};
		}
		template <typename S>
		static inline constexpr Matrix4<T> scale(S s)
		{
			return scale(s, s, s);
		}
		template <typename S>
		static inline constexpr Matrix4<T> scale(const Vector3<S>& v)
		{
			return scale(v.x, v.y, v.z);
		}

		// graphics stuff
		/*template <typename S>
		static inline Matrix4<T> look_at(const Vector3<S>& eye, const Vector3<S>& front, const Vector3<S>& up)
		{
			Vector3<T> z = eye - front;
			Vector3<T> x = up.cross(z).normalized();
			Vector3<T> y = z.cross(x);

			return Matrix4<T>{
				x.x, x.y, x.z, -x.dot(eye),
					y.x, y.y, y.z, -y.dot(eye),
					z.x, z.y, z.z, -z.dot(eye),
					0, 0, 0, 1
			};
		}

		template <typename S>
		static inline Matrix4<T> perspective(S fov, S aspect, S near, S far)
		{
			const T yMax = near * tan(fov * static_cast<T>(pi<T>) / 360);
			const T yMin = -yMax;
			const T xMin = yMin * aspect;
			const T xMax = yMax * aspect;

			return frustum(xMin, xMax, yMin, yMax, near, far);
		}
		template <typename S>
		static inline Matrix4<T> frustum(S left, S right, S bottom, S top, S near, S far)
		{
			return Matrix4<T>{
				(2 * near) / (right - left), 0, (right + left) / (right - left), 0,
					0, (2 * near) / (top - bottom), (top + bottom) / (top - bottom), 0,
					0, 0, -(far + near) / (far - near), (-2 * far * near) / (far - near),
					0, 0, -1, 0
			};
		}
		template <typename S>
		static inline Matrix4<T> orthographic(S left, S right, S bottom, S top, S near, S far)
		{
			return Matrix4<T>{
				2 / (right - left), 0, 0, -(right + left) / (right - left),
					0, 2 / (top - bottom), 0, -(top + bottom) / (top - bottom),
					0, 0, -2 / (far - near), -(far + near) / (far - near),
					0, 0, 0, 1
			};
		}*/
	};

	using t3f = Transform3<float>;
	using t3 = Transform3<defaultType>;

} // namespace els
