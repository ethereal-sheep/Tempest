/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#include "elsHeader.h"
#include "elsCompare.h"
#include "elsVectorGeneric.h"

namespace els
{
	template <typename T>
	class Vector2
	{
	public:
		using Scalar = T;

		Scalar x;
		Scalar y;

		constexpr Vector2() : Vector2{ static_cast<Scalar>(0) } {}
		constexpr explicit Vector2(const Scalar& s) : Vector2{ static_cast<Scalar>(s), static_cast<Scalar>(s) } {}
		constexpr Vector2(const Scalar& _x, const Scalar& _y) : x{ _x }, y{ _y }{}
		constexpr Vector2(const Vector2& rhs) : Vector2{ rhs.x, rhs.y } {}
		constexpr Vector2& operator=(const Vector2&);


		constexpr Scalar* data();
		constexpr const Scalar* data() const;

		constexpr Scalar& operator[](unsigned int index);
		constexpr const Scalar& operator[](unsigned int index) const;

		constexpr bool operator==(const Vector2&) const;
		constexpr bool operator!=(const Vector2&) const;


		constexpr Vector2 operator-() const;
		constexpr Vector2& operator+=(const Vector2&);
		constexpr Vector2& operator-=(const Vector2&);
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector2& operator*=(S);
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector2& operator/=(S);

		template <typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr explicit operator Vector2<S>() const
		{
			return Vector2<S>{S(x), S(y)};
		}
		template <typename V>
		constexpr explicit operator V() const
		{
			using S = decltype(V::x);
			return V{ S(x), S(y) };
		}

		constexpr Vector2 cw_normal() const;
		constexpr Vector2 ccw_normal() const;
		constexpr Vector2 normalized() const;
		constexpr Vector2 projection(const Vector2&) const;
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector2 lerp(const Vector2&, S) const;

		constexpr Scalar cross(const Vector2&) const;
		constexpr Scalar dot(const Vector2&) const;
		constexpr Scalar distance2(const Vector2&) const;
		constexpr Scalar distance(const Vector2&) const;
		constexpr Scalar length2() const;
		constexpr Scalar length() const;
		constexpr Scalar proj_length(const Vector2&) const;
		constexpr Scalar angle(const Vector2&) const;

		constexpr void normalize();

		static const Vector2 i;
		static const Vector2 j;
		static const Vector2 zero;
	};
	// typedefs
	using vec2f = Vector2<float>;
	using vec2b = Vector2<bool>;
	using vec2 = Vector2 <defaultType>;

	// constants
	template <typename T>
	const Vector2<T> Vector2<T>::i{ static_cast<Scalar>(1), static_cast<Scalar>(0) };
	template <typename T>
	const Vector2<T> Vector2<T>::j{ static_cast<Scalar>(0), static_cast<Scalar>(1) };
	template <typename T>
	const Vector2<T> Vector2<T>::zero{};

	// static functions
	template <typename T = defaultType, typename TVec>
	constexpr Vector2<T> to_vec2(const TVec& rhs)
	{
		return Vector2<T>{
			static_cast<T>(rhs.x),
				static_cast<T>(rhs.y)
		};
	}

	//global operators
	template<typename T>
	constexpr Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		Vector2<T> sum = lhs;
		sum += rhs;
		return sum;
	}
	template<typename T>
	constexpr Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
	{
		Vector2<T> diff = lhs;
		diff -= rhs;
		return diff;
	}
	template<typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector2<T> operator*(const Vector2<T>& lhs, S rhs)
	{
		Vector2<T> prod = lhs;
		prod *= rhs;
		return prod;
	}
	template<typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector2<T> operator/(const Vector2<T>& lhs, S rhs)
	{
		Vector2<T> quot = lhs;
		quot /= rhs;
		return quot;
	}
	template<typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector2<T> operator*(S lhs, const Vector2<T>& rhs)
	{
		return rhs * lhs;
	}

	// member functions
	template<typename T>
	constexpr Vector2<T>& Vector2<T>::operator=(const Vector2<T>& rhs)
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
		}
		return *this;
	}

	template<typename T>
	constexpr typename Vector2<T>::Scalar* Vector2<T>::data()
	{
		return &x;
	}
	template<typename T>
	constexpr const typename Vector2<T>::Scalar* Vector2<T>::data() const
	{
		return &x;
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar& Vector2<T>::operator[](unsigned int index)
	{
		return data()[index];
	}
	template<typename T>
	constexpr const typename Vector2<T>::Scalar& Vector2<T>::operator[](unsigned int index) const
	{
		return data()[index];
	}
	template<typename T>
	constexpr bool Vector2<T>::operator==(const Vector2<T>& rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	template<typename T>
	constexpr bool Vector2<T>::operator!=(const Vector2<T>& rhs) const
	{
		return !(*this == rhs);
	}
	template<typename T>
	constexpr Vector2<T>& Vector2<T>::operator+=(const Vector2<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	template<typename T>
	constexpr Vector2<T>& Vector2<T>::operator-=(const Vector2<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	template<typename T>
	template<typename S, typename>
	constexpr Vector2<T>& Vector2<T>::operator*=(S rhs)
	{
		x *= static_cast<T>(rhs);
		y *= static_cast<T>(rhs);
		return *this;
	}
	template<typename T>
	template<typename S, typename>
	constexpr Vector2<T>& Vector2<T>::operator/=(S rhs)
	{
		x /= static_cast<T>(rhs);
		y /= static_cast<T>(rhs);
		return *this;
	}
	template<typename T>
	constexpr Vector2<T> Vector2<T>::operator-() const
	{
		return Vector2<T>{ -x, -y };
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::dot(const Vector2<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::cross(const Vector2<T>& rhs) const
	{
		return x * rhs.y - y * rhs.x;
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::distance2(const Vector2<T>& rhs) const
	{
		return (*this - rhs).length2();
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::distance(const Vector2<T>& rhs) const
	{
		return sqrt(distance2(rhs));
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::length2() const
	{
		return dot(*this);
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::length() const
	{
		return sqrt(length2());
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::proj_length(const Vector2<T>& rhs) const
	{
		return dot(rhs.normalized());
	}
	template<typename T>
	constexpr Vector2<T> Vector2<T>::cw_normal() const
	{
		return Vector2<T>{ y, -x };
	}
	template<typename T>
	constexpr Vector2<T> Vector2<T>::ccw_normal() const
	{
		return Vector2<T>{ -y, x };
	}
	template<typename T>
	constexpr Vector2<T> Vector2<T>::normalized() const
	{
		Vector2<T> norm = *this;
		norm /= length();
		return std::move(norm);
	}
	template<typename T>
	constexpr typename Vector2<T>::Scalar Vector2<T>::angle(const Vector2<T>& rhs) const
	{
		T denominator2 = length2() * rhs.length2();
		if (is_zero(denominator2))
			return 0;
		return acos(dot(rhs) / sqrt(denominator2));
	}
	template<typename T>
	constexpr Vector2<T> Vector2<T>::projection(const Vector2<T>& rhs) const
	{
		return proj_length(rhs) * rhs.normalized();
	}
	template<typename T>
	template<typename S, typename>
	constexpr Vector2<T> Vector2<T>::lerp(const Vector2<T>& b, S t) const
	{
		return *this * (1 - t) + b * t;
	}
	template<typename T>
	constexpr void Vector2<T>::normalize()
	{
		*this /= length();
	}

} // namespace els
