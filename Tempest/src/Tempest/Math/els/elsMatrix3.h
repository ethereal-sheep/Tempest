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

#include <array>
#include "elsHeader.h"
#include "elsVector2.h"
#include "elsVector3.h"
#include "elsMath.h"

namespace els
{
	template <typename T>
	union Matrix3
	{
		using Scalar = T;

		std::array<Scalar, 9> m;
		Scalar m2[3][3];

		constexpr Matrix3() : Matrix3{ I } {}
		constexpr explicit Matrix3(const Scalar& s) : Matrix3{ s * I } {}
		constexpr Matrix3(const Matrix3& rhs) : m{ rhs.m } {}
		constexpr Matrix3(
			const Scalar& _00, const Scalar& _01, const Scalar& _02,
			const Scalar& _10, const Scalar& _11, const Scalar& _12,
			const Scalar& _20, const Scalar& _21, const Scalar& _22)
			: m{ _00, _01, _02,
				 _10, _11, _12,
				 _20, _21, _22 } {}
		constexpr Matrix3& operator=(const Matrix3& rhs);

		constexpr Scalar* data();
		constexpr const Scalar* data() const;

		constexpr Scalar* operator[](unsigned int index);
		constexpr const Scalar* operator[](unsigned int index) const;

		constexpr bool operator==(const Matrix3& rhs) const;
		constexpr bool operator!=(const Matrix3& rhs) const;

		constexpr Matrix3& operator*=(const Matrix3& rhs);
		template <typename S>
		constexpr Matrix3& operator*=(const S& rhs);
		constexpr Matrix3& operator+=(const Matrix3& rhs);
		constexpr Matrix3& operator-=(const Matrix3& rhs);

		constexpr operator bool() const;

		constexpr Scalar det() const;

		constexpr Matrix3& identity();
		constexpr Matrix3& transpose();
		constexpr Matrix3& invert();

		constexpr Matrix3 transposed() const;
		constexpr Matrix3 inverse() const; // return zero matrix on fail

		static const Matrix3 I;
		static const Matrix3 zero;
	};

	// typedef
	using mat3f = Matrix3<float>;
	using mat3 = Matrix3<defaultType>;

	// constants
	template <typename T>
	const Matrix3<T> Matrix3<T>::zero{ static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0),
										static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0),
										static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0) };
	template <typename T>
	const Matrix3<T> Matrix3<T>::I{ static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0),
									static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0),
									static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1) };

	// static functions
	template <typename T>
	constexpr void identity(Matrix3<T>& rhs)
	{
		rhs.identity();
	}
	template <typename T>
	constexpr void transpose(Matrix3<T>& rhs)
	{
		rhs.transpose();
	}
	template <typename T>
	constexpr bool invert(Matrix3<T>& rhs)
	{
		return rhs.invert();
	}


	// global operators
	template <typename T>
	constexpr Matrix3<T> operator*(const Matrix3<T>& lhs, const Matrix3<T>& rhs)
	{
		Matrix3<T> temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T, typename S>
	constexpr Matrix3<T> operator*(const S& lhs, const Matrix3<T>& rhs)
	{
		Matrix3<T> temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T, typename S>
	constexpr Matrix3<T> operator*(const Matrix3<T>& lhs, const S& rhs)
	{
		Matrix3<T> temp = lhs;
		temp *= rhs;
		return temp;
	}
	template <typename T>
	constexpr Vector2<T> operator*(const Vector2<T>& lhs, const Matrix3<T>& rhs)
	{
		return rhs.transposed() * lhs;
	}
	template <typename T>
	constexpr Vector2<T> operator*(const Matrix3<T>& lhs, const Vector2<T>& rhs)
	{
		return Vector2<T>{
			(lhs.m[0] * rhs.x + lhs.m[1] * rhs.y + lhs.m[2]),
				(lhs.m[3] * rhs.x + lhs.m[4] * rhs.y + lhs.m[5])
		};
	}
	template <typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& lhs, const Matrix3<T>& rhs)
	{
		return rhs.transposed() * lhs;
	}
	template <typename T>
	constexpr Vector3<T> operator*(const Matrix3<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>{
			(lhs.m[0] * rhs.x + lhs.m[1] * rhs.y + lhs.m[2] * rhs.z),
				(lhs.m[3] * rhs.x + lhs.m[4] * rhs.y + lhs.m[5] * rhs.z),
				(lhs.m[6] * rhs.x + lhs.m[7] * rhs.y + lhs.m[8] * rhs.z)
		};
	}

	// member functions
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& rhs)
	{
		if (this != &rhs)
			m = rhs.m;
		return *this;
	}
	template <typename T>
	constexpr typename Matrix3<T>::Scalar* Matrix3<T>::data()
	{
		return &m2[0][0];
	}
	template <typename T>
	constexpr const typename Matrix3<T>::Scalar* Matrix3<T>::data() const
	{
		return &m2[0][0];
	}
	template <typename T>
	constexpr typename Matrix3<T>::Scalar* Matrix3<T>::operator[](unsigned int index)
	{
		return m2[index];
	}
	template <typename T>
	constexpr const typename Matrix3<T>::Scalar* Matrix3<T>::operator[](unsigned int index) const
	{
		return m2[index];
	}
	template <typename T>
	constexpr bool Matrix3<T>::operator==(const Matrix3<T>& rhs) const
	{
		for (size_t i = 0; i < m.size(); ++i)
		{
			if (m[i] != rhs.m[i])
				return false;
		}
		return true;
	}
	template <typename T>
	constexpr bool Matrix3<T>::operator!=(const Matrix3<T>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<T>& rhs)
	{
		Matrix3<T> lhs = *this;

		m[0] = (lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[3] + lhs.m[2] * rhs.m[6]);
		m[1] = (lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[7]);
		m[2] = (lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[8]);

		m[3] = (lhs.m[3] * rhs.m[0] + lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[6]);
		m[4] = (lhs.m[3] * rhs.m[1] + lhs.m[4] * rhs.m[4] + lhs.m[5] * rhs.m[7]);
		m[5] = (lhs.m[3] * rhs.m[2] + lhs.m[4] * rhs.m[5] + lhs.m[5] * rhs.m[8]);

		m[6] = (lhs.m[6] * rhs.m[0] + lhs.m[7] * rhs.m[3] + lhs.m[8] * rhs.m[6]);
		m[7] = (lhs.m[6] * rhs.m[1] + lhs.m[7] * rhs.m[4] + lhs.m[8] * rhs.m[7]);
		m[8] = (lhs.m[6] * rhs.m[2] + lhs.m[7] * rhs.m[5] + lhs.m[8] * rhs.m[8]);

		return *this;
	}
	template <typename T>
	template <typename S>
	constexpr Matrix3<T>& Matrix3<T>::operator*=(const S& rhs)
	{
		for (auto& s : m)
			s *= rhs;
		return *this;
	}
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T>& rhs)
	{
		for (size_t i = 0; i < m.size(); ++i)
			m[i] += rhs.m[i];

		return *this;
	}
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T>& rhs)
	{
		for (size_t i = 0; i < m.size(); ++i)
			m[i] -= rhs.m[i];

		return *this;
	}
	template <typename T>
	constexpr Matrix3<T>::operator bool() const
	{
		return *this == zero;
	}
	template <typename T>
	constexpr typename Matrix3<T>::Scalar Matrix3<T>::det() const
	{
		return (m[0] * (m[4] * m[8] - m[5] * m[7]) - (m[1] * (m[3] * m[8] - m[5] * m[6])) + (m[2] * (m[3] * m[7] - m[4] * m[6])));
	}
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::identity()
	{
		*this = I;
		return *this;
	}
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::transpose()
	{
		*this = transposed();
		return *this;
	}
	template <typename T>
	constexpr Matrix3<T>& Matrix3<T>::invert()
	{
		Scalar determinant = det();
		if (is_zero(determinant))
		{
			*this = zero;
			return *this;
		}
		Matrix3<T> temp = *this;

		m[0] = (temp.m[4] * temp.m[8] - temp.m[7] * temp.m[5]) / determinant;
		m[1] = (temp.m[2] * temp.m[7] - temp.m[1] * temp.m[8]) / determinant;
		m[2] = (temp.m[1] * temp.m[5] - temp.m[2] * temp.m[4]) / determinant;

		m[3] = (temp.m[5] * temp.m[6] - temp.m[3] * temp.m[8]) / determinant;
		m[4] = (temp.m[0] * temp.m[8] - temp.m[2] * temp.m[6]) / determinant;
		m[5] = (temp.m[3] * temp.m[2] - temp.m[0] * temp.m[5]) / determinant;

		m[6] = (temp.m[3] * temp.m[7] - temp.m[6] * temp.m[4]) / determinant;
		m[7] = (temp.m[6] * temp.m[1] - temp.m[0] * temp.m[7]) / determinant;
		m[8] = (temp.m[0] * temp.m[4] - temp.m[3] * temp.m[1]) / determinant;

		return *this;
	}
	template <typename T>
	constexpr Matrix3<T> Matrix3<T>::transposed() const
	{
		return Matrix3<T>{
			m2[0][0], m2[1][0], m2[2][0],
				m2[0][1], m2[1][1], m2[2][1],
				m2[0][2], m2[1][2], m2[2][2]
		};
	}
	template <typename T>
	constexpr Matrix3<T> Matrix3<T>::inverse() const
	{
		Matrix3<T> temp = *this;
		return temp.invert();
	}

} // namespace els
