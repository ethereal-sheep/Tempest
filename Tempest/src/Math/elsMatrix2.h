/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#include <array>
#include "elsHeader.h"
#include "elsVector2.h"
#include "elsMath.h"

namespace els
{
	template <typename T>
	union Matrix2
	{
		using Scalar = T;

		std::array<Scalar, 4> m;
		Scalar m2[2][2];

		constexpr Matrix2() : Matrix2{ I } {}
		constexpr explicit Matrix2(const Scalar& s) : Matrix2{ s * I } {}
		constexpr Matrix2(const Matrix2& rhs) : m{ rhs.m } {}
		constexpr Matrix2(
			const Scalar& _00, const Scalar& _01,
			const Scalar& _10, const Scalar& _11)
			: m{ _00, _01,
				 _10, _11} {}
		constexpr Matrix2& operator=(const Matrix2& rhs);

		constexpr Scalar* data();
		constexpr const Scalar* data() const;

		constexpr Scalar* operator[](unsigned int index);
		constexpr const Scalar* operator[](unsigned int index) const;

		constexpr bool operator==(const Matrix2& rhs) const;
		constexpr bool operator!=(const Matrix2& rhs) const;

		constexpr Matrix2& operator*=(const Matrix2& rhs);
		template <typename S>
		constexpr Matrix2& operator*=(const S& rhs);
		constexpr Matrix2& operator+=(const Matrix2& rhs);
		constexpr Matrix2& operator-=(const Matrix2& rhs);

		constexpr operator bool() const;

		constexpr Scalar det() const;

		constexpr Matrix2& identity();
		constexpr Matrix2& transpose();
		constexpr Matrix2& invert();

		constexpr Matrix2 transposed() const;
		constexpr Matrix2 inverse() const; // return zero matrix on fail

		static const Matrix2 I;
		static const Matrix2 zero;
	};

	// typedef
	using mat2f = Matrix2<float>;
	using mat2 = Matrix2<defaultType>;

	// constants
	template <typename T>
	const Matrix2<T> Matrix2<T>::zero{ static_cast<Scalar>(0), static_cast<Scalar>(0),
										static_cast<Scalar>(0), static_cast<Scalar>(0)};
	template <typename T>
	const Matrix2<T> Matrix2<T>::I{ static_cast<Scalar>(1), static_cast<Scalar>(0),
									static_cast<Scalar>(0), static_cast<Scalar>(1)};

	// static functions
	template <typename T>
	constexpr void identity(Matrix2<T>& rhs)
	{
		rhs.identity();
	}
	template <typename T>
	constexpr void transpose(Matrix2<T>& rhs)
	{
		rhs.transpose();
	}
	template <typename T>
	constexpr bool invert(Matrix2<T>& rhs)
	{
		return rhs.invert();
	}


	// global operators
	template <typename T>
	constexpr Matrix2<T> operator*(const Matrix2<T>& lhs, const Matrix2<T>& rhs)
	{
		Matrix2<T> temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T, typename S>
	constexpr Matrix2<T> operator*(const S& lhs, const Matrix2<T>& rhs)
	{
		Matrix2<T> temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T, typename S>
	constexpr Matrix2<T> operator*(const Matrix2<T>& lhs, const S& rhs)
	{
		Matrix2<T> temp = lhs;
		temp *= rhs;
		return temp;
	}
	template <typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& lhs, const Matrix2<T>& rhs)
	{
		return rhs.transposed() * lhs;
	}
	template <typename T>
	constexpr Vector2<T> operator*(const Matrix2<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>{
			(lhs.m[0] * rhs.x + lhs.m[1] * rhs.y),
				(lhs.m[2] * rhs.x + lhs.m[3] * rhs.y)
		};
	}

	// member functions
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::operator=(const Matrix2<T>& rhs)
	{
		if (this != &rhs)
			m = rhs.m;
		return *this;
	}
	template <typename T>
	constexpr typename Matrix2<T>::Scalar* Matrix2<T>::data()
	{
		return &m2[0][0];
	}
	template <typename T>
	constexpr const typename Matrix2<T>::Scalar* Matrix2<T>::data() const
	{
		return &m2[0][0];
	}
	template <typename T>
	constexpr typename Matrix2<T>::Scalar* Matrix2<T>::operator[](unsigned int index)
	{
		return m2[index];
	}
	template <typename T>
	constexpr const typename Matrix2<T>::Scalar* Matrix2<T>::operator[](unsigned int index) const
	{
		return m2[index];
	}
	template <typename T>
	constexpr bool Matrix2<T>::operator==(const Matrix2<T>& rhs) const
	{
		for (size_t i = 0; i < m.size(); ++i)
		{
			if (m[i] != rhs.m[i])
				return false;
		}
		return true;
	}
	template <typename T>
	constexpr bool Matrix2<T>::operator!=(const Matrix2<T>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::operator*=(const Matrix2<T>& rhs)
	{
		Matrix2<T> lhs = *this;

		m[0] = (lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[2]);
		m[1] = (lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[3]);

		m[2] = (lhs.m[2] * rhs.m[0] + lhs.m[3] * rhs.m[2]);
		m[3] = (lhs.m[2] * rhs.m[1] + lhs.m[3] * rhs.m[3]);

		return *this;
	}
	template <typename T>
	template <typename S>
	constexpr Matrix2<T>& Matrix2<T>::operator*=(const S& rhs)
	{
		for (auto& s : m)
			s *= rhs;
		return *this;
	}
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::operator+=(const Matrix2<T>& rhs)
	{
		for (size_t i = 0; i < m.size(); ++i)
			m[i] += rhs.m[i];

		return *this;
	}
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::operator-=(const Matrix2<T>& rhs)
	{
		for (size_t i = 0; i < m.size(); ++i)
			m[i] -= rhs.m[i];

		return *this;
	}
	template <typename T>
	constexpr Matrix2<T>::operator bool() const
	{
		return *this == zero;
	}
	template <typename T>
	constexpr typename Matrix2<T>::Scalar Matrix2<T>::det() const
	{
		return m[0] * m[3] - m[1] * m[2];
	}
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::identity()
	{
		*this = I;
		return *this;
	}
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::transpose()
	{
		*this = transposed();
		return *this;
	}
	template <typename T>
	constexpr Matrix2<T>& Matrix2<T>::invert()
	{
		Scalar determinant = det();
		if (isZero(determinant))
		{
			*this = zero;
			return *this;
		}
		Matrix2<T> temp = *this;

		m[0] = temp.m[3] / determinant;
		m[1] = -temp.m[1] / determinant;

		m[2] = -temp.m[2] / determinant;
		m[3] = temp.m[1] / determinant;

		return *this;
	}
	template <typename T>
	constexpr Matrix2<T> Matrix2<T>::transposed() const
	{
		return Matrix2<T>{
				m2[0][0], m2[1][0],
				m2[0][1], m2[1][1]
		};
	}
	template <typename T>
	constexpr Matrix2<T> Matrix2<T>::inverse() const
	{
		Matrix2<T> temp = *this;
		return temp.invert();
	}

} // namespace els
