/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once
#pragma warning(disable : 4201) // suppress unnamed struct

#include "elsHeader.h"
#include "elsCompare.h"
#include "elsVectorGeneric.h"

namespace els
{
	template<typename T>
	class Vector3
	{
	public:
		using Scalar = T;

		union
		{
			struct
			{
				Scalar x;
				Scalar y;
				Scalar z;
			};
			struct
			{
				Scalar r;
				Scalar g;
				Scalar b;
			};
		};

		constexpr Vector3() : Vector3{ static_cast<Scalar>(0) } {}
		constexpr explicit Vector3(const Scalar& s) : Vector3{ static_cast<Scalar>(s), static_cast<Scalar>(s), static_cast<Scalar>(s) } {}
		constexpr Vector3(const Scalar& _x, const Scalar& _y, const Scalar& _z) : x{ _x }, y{ _y }, z{ _z }{}
		constexpr Vector3(const Vector3& rhs) : Vector3{ rhs.x, rhs.y, rhs.z } {}
		constexpr Vector3& operator=(const Vector3&);


		constexpr Scalar* data();
		constexpr const Scalar* data() const;

		constexpr Scalar& operator[](unsigned int index);
		constexpr const Scalar& operator[](unsigned int index) const;

		constexpr bool operator==(const Vector3&) const;
		constexpr bool operator!=(const Vector3&) const;

		constexpr Vector3 operator-() const;
		constexpr Vector3& operator+=(const Vector3&);
		constexpr Vector3& operator-=(const Vector3&);
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector3& operator*=(S);
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector3& operator/=(S);

		template <typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr explicit operator Vector3<S>() const
		{
			return Vector3<S>{S(x), S(y), S(z)};
		}
		template <typename V>
		constexpr explicit operator V() const
		{
			using S = decltype(V::x);
			return V{ S(x), S(y), S(z) };
		}

		constexpr Vector3 normalized() const;
		constexpr Vector3 projection(const Vector3&) const;
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector3 lerp(const Vector3&, S) const;
		constexpr Vector3 cross(const Vector3&) const;

		constexpr Scalar dot(const Vector3&) const;
		constexpr Scalar distance2(const Vector3&) const;
		constexpr Scalar distance(const Vector3&) const;
		constexpr Scalar length2() const;
		constexpr Scalar length() const;
		constexpr Scalar proj_length(const Vector3&) const;
		constexpr Scalar angle(const Vector3&) const;

		constexpr void normalize();


		static const Vector3 i;
		static const Vector3 j;
		static const Vector3 k;
		static const Vector3 zero;
	};

	// typedefs
	using vec3f = Vector3<float>;
	using vec3b = Vector3<bool>;
	using vec3 = Vector3<defaultType>;
	using rgb = Vector3<uint8_t>;

	// constants
	template <typename T>
	const Vector3<T> Vector3<T>::i{ static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0) };
	template <typename T>
	const Vector3<T> Vector3<T>::j{ static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0) };
	template <typename T>
	const Vector3<T> Vector3<T>::k{ static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1) };
	template <typename T>
	const Vector3<T> Vector3<T>::zero{};

	// static functions
	template <typename T = defaultType, typename TVec>
	constexpr Vector3<T> to_vec3(const TVec& rhs)
	{
		return Vector3<T>{
			static_cast<T>(rhs.x),
				static_cast<T>(rhs.y),
				static_cast<T>(rhs.z)
		};
	}

	//global operators
	template <typename T>
	constexpr Vector3<T> operator+(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		Vector3<T> sum = lhs;
		sum += rhs;
		return sum;
	}
	template <typename T>
	constexpr Vector3<T> operator-(const Vector3<T>& lhs, const Vector3<T>& rhs)
	{
		Vector3<T> diff = lhs;
		diff -= rhs;
		return diff;
	}
	template <typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector3<T> operator*(const Vector3<T>& lhs, const S& rhs)
	{
		Vector3<T> prod = lhs;
		prod *= rhs;
		return prod;
	}
	template <typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector3<T> operator/(const Vector3<T>& lhs, const S& rhs)
	{
		Vector3<T> quot = lhs;
		quot /= rhs;
		return quot;
	}
	template <typename T>
	constexpr Vector3<T> operator*(const typename Vector3<T>::Scalar& lhs, const Vector3<T>& rhs)
	{
		return rhs * lhs;
	}

	// member functions
	template<typename T>
	constexpr typename Vector3<T>::Scalar* Vector3<T>::data()
	{
		return &x;
	}
	template<typename T>
	constexpr const typename Vector3<T>::Scalar* Vector3<T>::data() const
	{
		return &x;
	}
	template<typename T>
	constexpr typename Vector3<T>::Scalar& Vector3<T>::operator[](unsigned int index)
	{
		return data()[index];
	}
	template<typename T>
	constexpr const typename Vector3<T>::Scalar& Vector3<T>::operator[](unsigned int index) const
	{
		return data()[index];
	}
	template <typename T>
	constexpr Vector3<T>& Vector3<T>::operator=(const Vector3<T>& rhs)
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
		}
		return *this;
	}
	
	template <typename T>
	constexpr bool Vector3<T>::operator==(const Vector3<T>& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	template <typename T>
	constexpr bool Vector3<T>::operator!=(const Vector3<T>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename T>
	constexpr Vector3<T>& Vector3<T>::operator+=(const Vector3<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	template <typename T>
	constexpr Vector3<T>& Vector3<T>::operator-=(const Vector3<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	template <typename T>
	template <typename S, typename>
	constexpr Vector3<T>& Vector3<T>::operator*=(S rhs)
	{
		x *= static_cast<T>(rhs);
		y *= static_cast<T>(rhs);
		z *= static_cast<T>(rhs);
		return *this;
	}
	template <typename T>
	template <typename S, typename>
	constexpr Vector3<T>& Vector3<T>::operator/=(S rhs)
	{
		x /= static_cast<T>(rhs);
		y /= static_cast<T>(rhs);
		z /= static_cast<T>(rhs);
		return *this;
	}
	template <typename T>
	constexpr Vector3<T> Vector3<T>::operator-() const
	{
		return Vector3<T>{ -x, -y, -z };
	}
	template <typename T>
	constexpr Vector3<T> Vector3<T>::normalized() const
	{
		Vector3<T> norm = *this;
		norm /= length();
		return std::move(norm);
	}
	template <typename T>
	constexpr Vector3<T> Vector3<T>::projection(const Vector3<T>& rhs) const
	{
		return proj_length(rhs) * rhs.normalized();
	}
	template <typename T>
	template<typename S, typename>
	constexpr Vector3<T> Vector3<T>::lerp(const Vector3<T>& b, S t) const
	{
		return *this * (1 - t) + b * t;
	}
	template <typename T>
	constexpr Vector3<T> Vector3<T>::cross(const Vector3<T>& rhs) const
	{

		return Vector3<T>{
			y* rhs.z - z * rhs.y,
				-(x * rhs.z - z * rhs.x),
				x* rhs.y - y * rhs.x };
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::dot(const Vector3<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::distance2(const Vector3<T>& rhs) const
	{
		return (*this - rhs).length2();
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::distance(const Vector3<T>& rhs) const
	{
		return sqrt(distance2(rhs));
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::length2() const
	{
		return dot(*this);
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::length() const
	{
		return sqrt(length2());
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::proj_length(const Vector3<T>& rhs) const
	{
		return dot(rhs.normalized());
	}
	template <typename T>
	constexpr typename Vector3<T>::Scalar Vector3<T>::angle(const Vector3<T>& rhs) const
	{
		T denominator2 = length2() * rhs.length2();
		if (is_zero(denominator2))
			return 0;
		return acos(dot(rhs) / sqrt(denominator2));
	}
	template <typename T>
	constexpr void Vector3<T>::normalize()
	{
		*this /= length();
	}

} // namespace els
