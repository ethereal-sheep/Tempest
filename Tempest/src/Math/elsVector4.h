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
	class Vector4
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
				Scalar w;
			};

			struct
			{
				Scalar r;
				Scalar g;
				Scalar b;
				Scalar a;
			};
		};


		constexpr Vector4() : Vector4{ static_cast<Scalar>(0) } {}
		constexpr explicit Vector4(const Scalar& s) : Vector4{ static_cast<Scalar>(s), static_cast<Scalar>(s), static_cast<Scalar>(s), static_cast<Scalar>(s) } {}
		constexpr Vector4(const Scalar& _x, const Scalar& _y, const Scalar& _z, const Scalar& _w) : x{ _x }, y{ _y }, z{ _z }, w{ _w }{}
		constexpr Vector4(const Vector4& rhs) : Vector4{ rhs.x, rhs.y, rhs.z, rhs.w } {}
		constexpr Vector4& operator=(const Vector4&);

		constexpr Scalar* data();
		constexpr const Scalar* data() const;

		constexpr Scalar& operator[](unsigned int index);
		constexpr const Scalar& operator[](unsigned int index) const;

		constexpr bool operator==(const Vector4&) const;
		constexpr bool operator!=(const Vector4&) const;

		constexpr Vector4 operator-() const;
		constexpr Vector4& operator+=(const Vector4&);
		constexpr Vector4& operator-=(const Vector4&);
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector4& operator*=(S);
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector4& operator/=(S);

		template <typename S>
		constexpr explicit operator Vector4<S>() const
		{
			return Vector4<S>{S(x), S(y), S(z), S(w)};
		}
		template <typename V>
		constexpr explicit operator V() const
		{
			using S = decltype(V::x);
			return V{ S(x), S(y), S(z), S(w) };
		}

		constexpr Vector4 normalized() const;
		constexpr Vector4 projection(const Vector4&) const;
		template<typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
		constexpr Vector4 lerp(const Vector4&, S) const;

		constexpr Scalar dot(const Vector4&) const;
		constexpr Scalar distance2(const Vector4&) const;
		constexpr Scalar distance(const Vector4&) const;
		constexpr Scalar length2() const;
		constexpr Scalar length() const;
		constexpr Scalar proj_length(const Vector4&) const;
		constexpr Scalar angle(const Vector4&) const;

		constexpr void normalize();

		static const Vector4 i;
		static const Vector4 j;
		static const Vector4 k;
		static const Vector4 l;
		static const Vector4 zero;
	};

	// typedefs
	using vec4f = Vector4<float>;
	using vec4b = Vector4<bool>;
	using vec4 = Vector4<defaultType>;
	using rgba = Vector4<uint8_t>;

	// constants
	template <typename T>
	const Vector4<T> Vector4<T>::i{ static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0) };
	template <typename T>
	const Vector4<T> Vector4<T>::j{ static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0) };
	template <typename T>
	const Vector4<T> Vector4<T>::k{ static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0) };
	template <typename T>
	const Vector4<T> Vector4<T>::l{ static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1) };
	template <typename T>
	const Vector4<T> Vector4<T>::zero{};

	// static functions
	template <typename T = defaultType, typename TVec>
	Vector4<T> to_vec4(const TVec& rhs)
	{
		return Vector4<T>{
			static_cast<T>(rhs.x),
			static_cast<T>(rhs.y),
			static_cast<T>(rhs.z),
			static_cast<T>(rhs.w),
		};
	}


	//global operators
	template <typename T>
	constexpr Vector4<T> operator+(const Vector4<T>& lhs, const Vector4<T>& rhs)
	{
		Vector4<T> sum = lhs;
		sum += rhs;
		return sum;
	}
	template <typename T>
	constexpr Vector4<T> operator-(const Vector4<T>& lhs, const Vector4<T>& rhs)
	{
		Vector4<T> diff = lhs;
		diff -= rhs;
		return diff;
	}
	template <typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector4<T> operator*(const Vector4<T>& lhs, S rhs)
	{
		Vector4<T> prod = lhs;
		prod *= rhs;
		return prod;
	}
	template <typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector4<T> operator/(const Vector4<T>& lhs, S rhs)
	{
		Vector4<T> quot = lhs;
		quot /= rhs;
		return quot;
	}
	template <typename T, typename S, typename = std::enable_if_t<std::is_arithmetic<S>::value>>
	constexpr Vector4<T> operator*(S lhs, const Vector4<T>& rhs)
	{
		return rhs * lhs;
	}

	// member functions
	template<typename T>
	constexpr typename Vector4<T>::Scalar* Vector4<T>::data()
	{
		return &x;
	}
	template<typename T>
	constexpr const typename Vector4<T>::Scalar* Vector4<T>::data() const
	{
		return &x;
	}
	template<typename T>
	constexpr typename Vector4<T>::Scalar& Vector4<T>::operator[](unsigned int index)
	{
		return data()[index];
	}
	template<typename T>
	constexpr const typename Vector4<T>::Scalar& Vector4<T>::operator[](unsigned int index) const
	{
		return data()[index];
	}
	template <typename T>
	constexpr Vector4<T>& Vector4<T>::operator=(const Vector4<T>& rhs)
	{
		if (this != &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			z = rhs.z;
			w = rhs.w;
		}
		return *this;
	}
	
	template <typename T>
	constexpr bool Vector4<T>::operator==(const Vector4<T>& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}
	template <typename T>
	constexpr bool Vector4<T>::operator!=(const Vector4<T>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename T>
	constexpr Vector4<T>& Vector4<T>::operator+=(const Vector4<T>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	template <typename T>
	constexpr Vector4<T>& Vector4<T>::operator-=(const Vector4<T>& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}
	template <typename T>
	template <typename S, typename>
	constexpr Vector4<T>& Vector4<T>::operator*=(S rhs)
	{
		x *= static_cast<T>(rhs);
		y *= static_cast<T>(rhs);
		z *= static_cast<T>(rhs);
		w *= static_cast<T>(rhs);
		return *this;
	}
	template <typename T>
	template <typename S, typename>
	constexpr Vector4<T>& Vector4<T>::operator/=(S rhs)
	{
		x /= static_cast<T>(rhs);
		y /= static_cast<T>(rhs);
		z /= static_cast<T>(rhs);
		w /= static_cast<T>(rhs);
		return *this;
	}
	template <typename T>
	constexpr Vector4<T> Vector4<T>::operator-() const
	{
		return Vector4<T>{ -x, -y, -z, -w };
	}
	template <typename T>
	constexpr Vector4<T> Vector4<T>::normalized() const
	{
		Vector4<T> norm = *this;
		norm /= length();
		return std::move(norm);
	}
	template <typename T>
	constexpr Vector4<T> Vector4<T>::projection(const Vector4<T>& rhs) const
	{
		return proj_length(rhs) * rhs.normalized();
	}
	template <typename T>
	template <typename S, typename>
	constexpr Vector4<T> Vector4<T>::lerp(const Vector4<T>& b, S t) const
	{
		return *this * (1 - t) + b * t;
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::dot(const Vector4<T>& rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w;
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::distance2(const Vector4<T>& rhs) const
	{
		return (*this - rhs).length2();
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::distance(const Vector4<T>& rhs) const
	{
		return sqrt(distance2(rhs));
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::length2() const
	{
		return dot(*this);
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::length() const
	{
		return sqrt(length2());
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::proj_length(const Vector4<T>& rhs) const
	{
		return dot(rhs.normalized());
	}
	template <typename T>
	constexpr typename Vector4<T>::Scalar Vector4<T>::angle(const Vector4<T>& rhs) const
	{
		T denominator2 = length2() * rhs.length2();
		if (is_zero(denominator2))
			return 0;
		return acos(dot(rhs) / sqrt(denominator2));
	}
	template <typename T>
	constexpr void Vector4<T>::normalize()
	{
		*this /= length();
	}

} // namespace els
