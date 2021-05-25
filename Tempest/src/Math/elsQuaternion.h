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
#include "elsMatrix4.h"

namespace els
{
	template <typename T>
	class Quaternion
	{
	public:
		using Scalar = T;

		Scalar x;
		Scalar y;
		Scalar z;
		Scalar w;

		constexpr Quaternion()
			: Quaternion{ static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0) } {}
		constexpr Quaternion(const Scalar& i, const Scalar& j, const Scalar& k, const Scalar& s) : x{ i }, y{ j }, z{ k }, w{ s } {}
		constexpr Quaternion(const Quaternion& rhs) : x{ rhs.x }, y{ rhs.y }, z{ rhs.z }, w{ rhs.w }{}
		constexpr Quaternion(const Vector3<T>& axis, const Scalar& angle)
			: Quaternion{}
		{
			set_rotation(axis, angle);
		}
		constexpr Quaternion(const Scalar& yaw, const Scalar& pitch, const Scalar& roll)
			: Quaternion{}
		{
			set_euler(yaw, pitch, roll);
		}

		constexpr Quaternion& operator=(const Quaternion&);
		constexpr Quaternion& operator+=(const Quaternion&);
		constexpr Quaternion& operator-=(const Quaternion&);
		constexpr Quaternion& operator*=(const Quaternion&);

		constexpr Quaternion& operator*=(const Scalar&);
		constexpr Quaternion& operator/=(const Scalar&);

		constexpr Quaternion operator-() const;

		constexpr explicit operator Vector3<T>() const
		{
			return Vector3<T>{ x, y, z };
		}
		constexpr explicit operator Vector4<T>() const
		{
			return Vector4<T>{ x, y, z, w };
		}

		constexpr const Scalar* data() const;
		constexpr Quaternion normalized() const;
		constexpr Quaternion conjugate() const;
		constexpr Quaternion inverse() const;
		template <typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Quaternion slerp(const Quaternion&, S t) const;

		constexpr Scalar dot(const Quaternion&) const;
		constexpr Scalar length2() const;
		constexpr Scalar length() const;

		constexpr Matrix4<T> to_mtx4() const;

		constexpr void normalize();
		constexpr void invert();

		constexpr void set_rotation(const Vector3<T>& axis, const Scalar& angle);

		constexpr void set_euler_yxz(const Scalar& yaw, const Scalar& pitch, const Scalar& roll);
		constexpr void set_euler_zyx(const Scalar& yaw, const Scalar& pitch, const Scalar& roll);
		constexpr void set_euler(const Scalar& yaw, const Scalar& pitch, const Scalar& roll);

		constexpr Scalar yaw() const;
		constexpr Scalar pitch() const;
		constexpr Scalar roll() const;

		constexpr Quaternion look_at(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up);


	};
	//typedef 
	using quatf = Quaternion<float>;
	using quat = Quaternion<defaultType>;

	// global operators
	template <typename T>
	constexpr Quaternion<T> operator+(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		Quaternion temp = lhs;
		temp += rhs;
		return temp;
	}
	template <typename T>
	constexpr Quaternion<T> operator-(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		Quaternion temp = lhs;
		temp -= rhs;
		return temp;
	}
	template <typename T>
	constexpr Quaternion<T> operator*(const Quaternion<T>& lhs, const Quaternion<T>& rhs)
	{
		Quaternion temp = lhs;
		temp *= rhs;
		return temp;
	}
	template <typename T>
	constexpr Quaternion<T> operator*(const typename Quaternion<T>::Scalar& lhs, const Quaternion<T>& rhs)
	{
		Quaternion temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T>
	constexpr Quaternion<T> operator*(const Quaternion<T>& lhs, const typename Quaternion<T>::Scalar& rhs)
	{
		Quaternion temp = lhs;
		temp *= rhs;
		return temp;
	}
	template <typename T>
	constexpr Quaternion<T> operator/(const Quaternion<T>& lhs, const typename Quaternion<T>::Scalar& rhs)
	{
		Quaternion temp = lhs;
		temp *= rhs;
		return temp;
	}
	template <typename T>
	constexpr Vector3<T>& operator*=(Vector3<T>& lhs, const Quaternion<T>& rhs)
	{
		lhs = lhs * rhs;
		return lhs;
	}
	template <typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& lhs, const Quaternion<T>& rhs)
	{
		Quaternion<T> temp{ lhs.x, lhs.y, lhs.z, static_cast<T>(0) };

		temp = rhs * temp * rhs.inverse();
		return static_cast<Vector3<T>>(temp);
	}

	// member functions
	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator=(const Quaternion<T>& rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		w = rhs.w;

		return *this;
	}
	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator+=(const Quaternion<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}
	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator-=(const Quaternion<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}
	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator*=(const Quaternion<T>& rhs)
	{
		Quaternion<T> temp = *this;
		y = temp.w * rhs.y + temp.y * rhs.w + temp.z * rhs.x - temp.x * rhs.z;
		z = temp.w * rhs.z + temp.z * rhs.w + temp.x * rhs.y - temp.y * rhs.x;
		w = temp.w * rhs.w - temp.x * rhs.x - temp.y * rhs.y - temp.z * rhs.z;
		x = temp.w * rhs.x + temp.x * rhs.w + temp.y * rhs.z - temp.z * rhs.y;

		return *this;
	}
	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator*=(const Scalar& rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		w *= rhs;

		return *this;
	}
	template <typename T>
	constexpr Quaternion<T>& Quaternion<T>::operator/=(const Scalar& rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;

		return *this;
	}
	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::operator-() const
	{
		return Quaternion<T>{ -w, -x, -y, -z };
	}
	template <typename T>
	constexpr const T* Quaternion<T>::data() const
	{
		return &x;
	}
	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::normalized() const
	{
		Quaternion temp = *this;
		temp /= length();
		return temp;
	}
	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::conjugate() const
	{
		return Quaternion<T>{ -x, -y, -z, w };
	}
	template <typename T>
	constexpr Quaternion<T> Quaternion<T>::inverse() const
	{
		return conjugate() / length();
	}
	template <typename T>
	template <typename S, typename>
	constexpr Quaternion<T> Quaternion<T>::slerp(const Quaternion<T>& b, S t) const
	{
		Scalar magnitude = sqrt(length2() * b.length2());
		if (is_zero(magnitude))
		{
			Scalar product = dot(b) / magnitude;
			if (abs(product) < static_cast<Scalar>(1))
			{
				Scalar sign = (product < 0) ? static_cast<Scalar>(-1) : static_cast<Scalar>(1);
				Scalar theta = acos(sign * product);
				Scalar s1 = sin(sign * t * theta);
				Scalar d = static_cast<Scalar>(1.0) / sin(theta);
				Scalar s0 = sin((static_cast<Scalar>(1.0) - t)* theta);

				return Quaternion(
					(x * s0 + b.x * s1) * d,
					(y * s0 + b.y * s1) * d,
					(z * s0 + b.z * s1) * d,
					(w * s0 + b.w * s1) * d);
			}
		}
		return *this;
	}
	template <typename T>
	constexpr typename Quaternion<T>::Scalar Quaternion<T>::dot(const Quaternion<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}
	template <typename T>
	constexpr typename Quaternion<T>::Scalar Quaternion<T>::length2() const
	{
		return dot(*this);
	}
	template <typename T>
	constexpr typename Quaternion<T>::Scalar Quaternion<T>::length() const
	{
		return sqrt(length2());
	}
	template <typename T>
	constexpr Matrix4<T> Quaternion<T>::to_mtx4() const
	{
		const T x2 = x * 2;
		const T y2 = y * 2;
		const T z2 = z * 2;

		const T xx = x * x2;
		const T xy = x * y2;
		const T xz = x * z2;
		const T yy = y * y2;
		const T yz = y * z2;
		const T zz = z * z2;

		const T xw = w * x2;
		const T yw = w * y2;
		const T zw = w * z2;

		return Matrix4<T>{
			1 - (yy + zz), xy - zw, xz + yw, 0,
				xy + zw, 1 - (xx + zz), yz - xw, 0,
				xz - yw, yz + xw, 1 - (xx + yy), 0,
				0, 0, 0, 1
		};
	}
	template <typename T>
	constexpr void Quaternion<T>::normalize()
	{
		*this /= length();
	}
	template <typename T>
	constexpr void Quaternion<T>::invert()
	{
		*this = inverse();
	}
	template <typename T>
	constexpr void Quaternion<T>::set_rotation(const Vector3<T>& axis, const Scalar& angle)
	{
		Scalar s = sin(angle * static_cast<Scalar>(0.5)) / axis.length();

		x = axis.x * s;
		y = axis.y * s;
		z = axis.z * s;
		w = cos(angle * static_cast<Scalar>(0.5));
	}
	template <typename T>
	constexpr void Quaternion<T>::set_euler_yxz(const Scalar& yaw, const Scalar& pitch, const Scalar& roll)
	{
		Scalar halfYaw = static_cast<Scalar>(yaw) * static_cast<Scalar>(0.5);
		Scalar halfPitch = static_cast<Scalar>(pitch) * static_cast<Scalar>(0.5);
		Scalar halfRoll = static_cast<Scalar>(roll) * static_cast<Scalar>(0.5);
		Scalar cosYaw = cos(halfYaw);
		Scalar sinYaw = sin(halfYaw);
		Scalar cosPitch = cos(halfPitch);
		Scalar sinPitch = sin(halfPitch);
		Scalar cosRoll = cos(halfRoll);
		Scalar sinRoll = sin(halfRoll);

		w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		x = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		y = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
		z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	}
	template <typename T>
	constexpr void Quaternion<T>::set_euler_zyx(const Scalar& yaw, const Scalar& pitch, const Scalar& roll)
	{
		Scalar halfYaw = static_cast<Scalar>(yaw) * static_cast<Scalar>(0.5);
		Scalar halfPitch = static_cast<Scalar>(pitch) * static_cast<Scalar>(0.5);
		Scalar halfRoll = static_cast<Scalar>(roll) * static_cast<Scalar>(0.5);
		Scalar cosYaw = cos(halfYaw);
		Scalar sinYaw = sin(halfYaw);
		Scalar cosPitch = cos(halfPitch);
		Scalar sinPitch = sin(halfPitch);
		Scalar cosRoll = cos(halfRoll);
		Scalar sinRoll = sin(halfRoll);

		w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;
		x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
		y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
		z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	}
	template <typename T>
	constexpr void Quaternion<T>::set_euler(const Scalar& yaw, const Scalar& pitch, const Scalar& roll)
	{
		set_euler_yxz(yaw, pitch, roll);
	}




} // namespace els
