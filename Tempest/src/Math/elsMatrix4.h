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
#include "elsVector3.h"
#include "elsVector4.h"
#include "elsMath.h"

namespace els
{
	template <typename T>
	union Matrix4
	{
		using Scalar = T;

		std::array<Scalar, 16> m;
		Scalar m2[4][4];

		constexpr Matrix4() : Matrix4{ I } {}
		constexpr explicit Matrix4(const Scalar& s) : Matrix4{ s * I } {}
		constexpr Matrix4(const Matrix4& rhs) : m{ rhs.m } {}
		constexpr Matrix4(
			const Scalar& _00, const Scalar& _01, const Scalar& _02, const Scalar& _03,
			const Scalar& _10, const Scalar& _11, const Scalar& _12, const Scalar& _13,
			const Scalar& _20, const Scalar& _21, const Scalar& _22, const Scalar& _23,
			const Scalar& _30, const Scalar& _31, const Scalar& _32, const Scalar& _33)
			: m{ _00, _01, _02, _03,
				 _10, _11, _12, _13,
				 _20, _21, _22, _23,
				 _30, _31, _32, _33, } {}

		constexpr Scalar* data();
		constexpr const Scalar* data() const;

		constexpr Scalar* operator[](unsigned int index);
		constexpr const Scalar* operator[](unsigned int index) const;

		constexpr bool operator==(const Matrix4& rhs) const;
		constexpr bool operator!=(const Matrix4& rhs) const;

		constexpr Matrix4& operator=(const Matrix4& rhs);
		constexpr Matrix4& operator*=(const Matrix4& rhs);
		template <typename S>
		constexpr Matrix4& operator*=(const S& rhs);
		constexpr Matrix4& operator+=(const Matrix4& rhs);
		constexpr Matrix4& operator-=(const Matrix4& rhs);

		constexpr operator bool() const;

		constexpr Scalar det() const;

		constexpr Matrix4& identity();
		constexpr Matrix4& transpose();
		constexpr Matrix4& invert();

		constexpr Matrix4 transposed() const;
		constexpr Matrix4 inverse() const;

		constexpr static Matrix4<T> look_at(const Vector3<T>& eye, const Vector3<T>& target, const Vector3<T>& up)
		{
			Vector4<T> zAxis = eye - target;
			Vector4<T> xAxis = up.cross(zAxis).normalize();
			Vector4<T> yAxis = zAxis.cross(xAxis);

			return Matrix4<T>{
				xAxis.x, yAxis.x, zAxis.x, 0,
					xAxis.y, yAxis.y, zAxis.y, 0,
					xAxis.z, yAxis.z, zAxis.z, 0,
					-dot(xAxis, eye), -dot(yAxis, eye), -dot(zAxis, eye), 1};
		}
		static const Matrix4 I;
		static const Matrix4 zero;
	};

	// typedef
	using mat4f = Matrix4<float>;
	using mat4 = Matrix4<defaultType>;

	// constants
	template <typename T>
	const Matrix4<T> Matrix4<T>::zero{ static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0),
										static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0),
										static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0),
										static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0) };
	template <typename T>
	const Matrix4<T> Matrix4<T>::I{ static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0),
									static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0), static_cast<Scalar>(0),
									static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1), static_cast<Scalar>(0),
									static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(0), static_cast<Scalar>(1) };

	// static functions
	template <typename T>
	constexpr void identity(Matrix4<T>& rhs)
	{
		rhs.identity();
	}
	template <typename T>
	constexpr void transpose(Matrix4<T>& rhs)
	{
		rhs.transpose();
	}
	template <typename T>
	constexpr bool invert(Matrix4<T>& rhs)
	{
		return rhs.invert();
	}

	// global operators
	template <typename T>
	constexpr Matrix4<T> operator*(const Matrix4<T>& lhs, const Matrix4<T>& rhs)
	{
		Matrix4<T> temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T, typename S>
	constexpr Matrix4<T> operator*(const S& lhs, const Matrix4<T>& rhs)
	{
		Matrix4<T> temp = rhs;
		temp *= lhs;
		return temp;
	}
	template <typename T, typename S>
	constexpr Matrix4<T> operator*(const Matrix4<T>& lhs, const S& rhs)
	{
		Matrix4<T> temp = lhs;
		temp *= rhs;
		return temp;
	}
	template <typename T>
	constexpr Vector3<T> operator*(const Vector3<T>& lhs, const Matrix4<T>& rhs)
	{
		return rhs.transposed() * lhs;
	}
	template <typename T>
	constexpr Vector3<T> operator*(const Matrix4<T>& lhs, const Vector3<T>& rhs)
	{
		return Vector3<T>{
			(lhs.m[0] * rhs.x + lhs.m[1] * rhs.y + lhs.m[2] * rhs.z + lhs.m[3]),
				(lhs.m[4] * rhs.x + lhs.m[5] * rhs.y + lhs.m[6] * rhs.z + lhs.m[7]),
				(lhs.m[8] * rhs.x + lhs.m[9] * rhs.y + lhs.m[10] * rhs.z + lhs.m[11])
		};
	}
	template <typename T>
	constexpr Vector4<T> operator*(const Vector4<T>& lhs, const Matrix4<T>& rhs)
	{
		return rhs.transposed() * lhs;
	}
	template <typename T>
	constexpr Vector4<T> operator*(const Matrix4<T>& lhs, const Vector4<T>& rhs)
	{
		return Vector4<T>{
			(lhs.m[0] * rhs.x + lhs.m[1] * rhs.y + lhs.m[2] * rhs.z + lhs.m[3] * rhs.w),
				(lhs.m[4] * rhs.x + lhs.m[5] * rhs.y + lhs.m[6] * rhs.z + lhs.m[7] * rhs.w),
				(lhs.m[8] * rhs.x + lhs.m[9] * rhs.y + lhs.m[10] * rhs.z + lhs.m[11] * rhs.w),
				(lhs.m[12] * rhs.x + lhs.m[13] * rhs.y + lhs.m[14] * rhs.z + lhs.m[15] * rhs.w)
		};
	}

	// member functions
	template <typename T>
	constexpr typename Matrix4<T>::Scalar* Matrix4<T>::data()
	{
		return &m2[0][0];
	}
	template <typename T>
	constexpr const typename Matrix4<T>::Scalar* Matrix4<T>::data() const
	{
		return &m2[0][0];
	}
	template <typename T>
	constexpr typename Matrix4<T>::Scalar* Matrix4<T>::operator[](unsigned int index)
	{
		return m2[index];
	}
	template <typename T>
	constexpr const typename Matrix4<T>::Scalar* Matrix4<T>::operator[](unsigned int index) const
	{
		return m2[index];
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& rhs)
	{
		if (this != &rhs)
			m = rhs.m;
		return *this;
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T>& rhs)
	{
		Matrix4<T> lhs = *this;

		m[0] = (lhs.m[0] * rhs.m[0] + lhs.m[1] * rhs.m[4] + lhs.m[2] * rhs.m[8] + lhs.m[3] * rhs.m[12]);
		m[1] = (lhs.m[0] * rhs.m[1] + lhs.m[1] * rhs.m[5] + lhs.m[2] * rhs.m[9] + lhs.m[3] * rhs.m[13]);
		m[2] = (lhs.m[0] * rhs.m[2] + lhs.m[1] * rhs.m[6] + lhs.m[2] * rhs.m[10] + lhs.m[3] * rhs.m[14]);
		m[3] = (lhs.m[0] * rhs.m[3] + lhs.m[1] * rhs.m[7] + lhs.m[2] * rhs.m[11] + lhs.m[3] * rhs.m[15]);

		m[4] = (lhs.m[4] * rhs.m[0] + lhs.m[5] * rhs.m[4] + lhs.m[6] * rhs.m[8] + lhs.m[7] * rhs.m[12]);
		m[5] = (lhs.m[4] * rhs.m[1] + lhs.m[5] * rhs.m[5] + lhs.m[6] * rhs.m[9] + lhs.m[7] * rhs.m[13]);
		m[6] = (lhs.m[4] * rhs.m[2] + lhs.m[5] * rhs.m[6] + lhs.m[6] * rhs.m[10] + lhs.m[7] * rhs.m[14]);
		m[7] = (lhs.m[4] * rhs.m[3] + lhs.m[5] * rhs.m[7] + lhs.m[6] * rhs.m[11] + lhs.m[7] * rhs.m[15]);

		m[8] = (lhs.m[8] * rhs.m[0] + lhs.m[9] * rhs.m[4] + lhs.m[10] * rhs.m[8] + lhs.m[11] * rhs.m[12]);
		m[9] = (lhs.m[8] * rhs.m[1] + lhs.m[9] * rhs.m[5] + lhs.m[10] * rhs.m[9] + lhs.m[11] * rhs.m[13]);
		m[10] = (lhs.m[8] * rhs.m[2] + lhs.m[9] * rhs.m[6] + lhs.m[10] * rhs.m[10] + lhs.m[11] * rhs.m[14]);
		m[11] = (lhs.m[8] * rhs.m[3] + lhs.m[9] * rhs.m[7] + lhs.m[10] * rhs.m[11] + lhs.m[11] * rhs.m[15]);

		m[12] = (lhs.m[12] * rhs.m[0] + lhs.m[13] * rhs.m[4] + lhs.m[14] * rhs.m[8] + lhs.m[15] * rhs.m[12]);
		m[13] = (lhs.m[12] * rhs.m[1] + lhs.m[13] * rhs.m[5] + lhs.m[14] * rhs.m[9] + lhs.m[15] * rhs.m[13]);
		m[14] = (lhs.m[12] * rhs.m[2] + lhs.m[13] * rhs.m[6] + lhs.m[14] * rhs.m[10] + lhs.m[15] * rhs.m[14]);
		m[15] = (lhs.m[12] * rhs.m[3] + lhs.m[13] * rhs.m[7] + lhs.m[14] * rhs.m[11] + lhs.m[15] * rhs.m[15]);

		return *this;
	}
	template <typename T>
	template <typename S>
	constexpr Matrix4<T>& Matrix4<T>::operator*=(const S& rhs)
	{
		for (auto& s : m)
			s *= rhs;

		return *this;
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T>& rhs)
	{
		for (size_t i = 0; i < m.size(); ++i)
			m[i] += rhs.m[i];

		return *this;
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T>& rhs)
	{
		for (size_t i = 0; i < m.size(); ++i)
			m[i] -= rhs.m[i];

		return *this;
	}
	template <typename T>
	constexpr bool Matrix4<T>::operator==(const Matrix4<T>& rhs) const
	{
		for (size_t i = 0; i < m.size(); ++i)
		{
			if (m[i] != rhs.m[i])
				return false;
		}
		return true;
	}
	template <typename T>
	constexpr bool Matrix4<T>::operator!=(const Matrix4<T>& rhs) const
	{
		return !(*this == rhs);
	}
	template <typename T>
	constexpr Matrix4<T>::operator bool() const
	{
		return *this != zero;
	}
	template <typename T>
	constexpr typename Matrix4<T>::Scalar Matrix4<T>::det() const
	{
		return
			m[12] * m[9] * m[6] * m[3] - m[8] * m[13] * m[6] * m[3] -
			m[12] * m[5] * m[10] * m[3] + m[4] * m[13] * m[10] * m[3] +
			m[8] * m[5] * m[14] * m[3] - m[4] * m[9] * m[14] * m[3] -
			m[12] * m[9] * m[2] * m[7] + m[8] * m[13] * m[2] * m[7] +
			m[12] * m[1] * m[10] * m[7] - m[0] * m[13] * m[10] * m[7] -
			m[8] * m[1] * m[14] * m[7] + m[0] * m[9] * m[14] * m[7] +
			m[12] * m[5] * m[2] * m[11] - m[4] * m[13] * m[2] * m[11] -
			m[12] * m[1] * m[6] * m[11] + m[0] * m[13] * m[6] * m[11] +
			m[4] * m[1] * m[14] * m[11] - m[0] * m[5] * m[14] * m[11] -
			m[8] * m[5] * m[2] * m[15] + m[4] * m[9] * m[2] * m[15] +
			m[8] * m[1] * m[6] * m[15] - m[0] * m[9] * m[6] * m[15] -
			m[4] * m[1] * m[10] * m[15] + m[0] * m[5] * m[10] * m[15];
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::identity()
	{
		*this = I;
		return *this;
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::transpose()
	{
		*this = transposed();
		return *this;
	}
	template <typename T>
	constexpr Matrix4<T>& Matrix4<T>::invert()
	{
		Scalar determinant = det();
		if (isZero(determinant))
		{
			*this = zero;
			return *this;
		}
		Matrix4<T> temp = *this;

		m[0] = temp[5] * temp[10] * temp[15] -
			temp[5] * temp[11] * temp[14] -
			temp[9] * temp[6] * temp[15] +
			temp[9] * temp[7] * temp[14] +
			temp[13] * temp[6] * temp[11] -
			temp[13] * temp[7] * temp[10];

		m[4] = -temp[4] * temp[10] * temp[15] +
			temp[4] * temp[11] * temp[14] +
			temp[8] * temp[6] * temp[15] -
			temp[8] * temp[7] * temp[14] -
			temp[12] * temp[6] * temp[11] +
			temp[12] * temp[7] * temp[10];

		m[8] = temp[4] * temp[9] * temp[15] -
			temp[4] * temp[11] * temp[13] -
			temp[8] * temp[5] * temp[15] +
			temp[8] * temp[7] * temp[13] +
			temp[12] * temp[5] * temp[11] -
			temp[12] * temp[7] * temp[9];

		m[12] = -temp[4] * temp[9] * temp[14] +
			temp[4] * temp[10] * temp[13] +
			temp[8] * temp[5] * temp[14] -
			temp[8] * temp[6] * temp[13] -
			temp[12] * temp[5] * temp[10] +
			temp[12] * temp[6] * temp[9];

		m[1] = -temp[1] * temp[10] * temp[15] +
			temp[1] * temp[11] * temp[14] +
			temp[9] * temp[2] * temp[15] -
			temp[9] * temp[3] * temp[14] -
			temp[13] * temp[2] * temp[11] +
			temp[13] * temp[3] * temp[10];

		m[5] = temp[0] * temp[10] * temp[15] -
			temp[0] * temp[11] * temp[14] -
			temp[8] * temp[2] * temp[15] +
			temp[8] * temp[3] * temp[14] +
			temp[12] * temp[2] * temp[11] -
			temp[12] * temp[3] * temp[10];

		m[9] = -temp[0] * temp[9] * temp[15] +
			temp[0] * temp[11] * temp[13] +
			temp[8] * temp[1] * temp[15] -
			temp[8] * temp[3] * temp[13] -
			temp[12] * temp[1] * temp[11] +
			temp[12] * temp[3] * temp[9];

		m[13] = temp[0] * temp[9] * temp[14] -
			temp[0] * temp[10] * temp[13] -
			temp[8] * temp[1] * temp[14] +
			temp[8] * temp[2] * temp[13] +
			temp[12] * temp[1] * temp[10] -
			temp[12] * temp[2] * temp[9];

		m[2] = temp[1] * temp[6] * temp[15] -
			temp[1] * temp[7] * temp[14] -
			temp[5] * temp[2] * temp[15] +
			temp[5] * temp[3] * temp[14] +
			temp[13] * temp[2] * temp[7] -
			temp[13] * temp[3] * temp[6];

		m[6] = -temp[0] * temp[6] * temp[15] +
			temp[0] * temp[7] * temp[14] +
			temp[4] * temp[2] * temp[15] -
			temp[4] * temp[3] * temp[14] -
			temp[12] * temp[2] * temp[7] +
			temp[12] * temp[3] * temp[6];

		m[10] = temp[0] * temp[5] * temp[15] -
			temp[0] * temp[7] * temp[13] -
			temp[4] * temp[1] * temp[15] +
			temp[4] * temp[3] * temp[13] +
			temp[12] * temp[1] * temp[7] -
			temp[12] * temp[3] * temp[5];

		m[14] = -temp[0] * temp[5] * temp[14] +
			temp[0] * temp[6] * temp[13] +
			temp[4] * temp[1] * temp[14] -
			temp[4] * temp[2] * temp[13] -
			temp[12] * temp[1] * temp[6] +
			temp[12] * temp[2] * temp[5];

		m[3] = -temp[1] * temp[6] * temp[11] +
			temp[1] * temp[7] * temp[10] +
			temp[5] * temp[2] * temp[11] -
			temp[5] * temp[3] * temp[10] -
			temp[9] * temp[2] * temp[7] +
			temp[9] * temp[3] * temp[6];

		m[7] = temp[0] * temp[6] * temp[11] -
			temp[0] * temp[7] * temp[10] -
			temp[4] * temp[2] * temp[11] +
			temp[4] * temp[3] * temp[10] +
			temp[8] * temp[2] * temp[7] -
			temp[8] * temp[3] * temp[6];

		m[11] = -temp[0] * temp[5] * temp[11] +
			temp[0] * temp[7] * temp[9] +
			temp[4] * temp[1] * temp[11] -
			temp[4] * temp[3] * temp[9] -
			temp[8] * temp[1] * temp[7] +
			temp[8] * temp[3] * temp[5];

		m[15] = temp[0] * temp[5] * temp[10] -
			temp[0] * temp[6] * temp[9] -
			temp[4] * temp[1] * temp[10] +
			temp[4] * temp[2] * temp[9] +
			temp[8] * temp[1] * temp[6] -
			temp[8] * temp[2] * temp[5];

		for (auto& i : m)
			i /= determinant;

		return *this;
	}
	template <typename T>
	constexpr Matrix4<T> Matrix4<T>::transposed() const
	{
		return Matrix4<T>{
			m2[0][0], m2[1][0], m2[2][0], m2[3][0],
				m2[0][1], m2[1][1], m2[2][1], m2[3][1],
				m2[0][2], m2[1][2], m2[2][2], m2[3][2],
				m2[0][3], m2[1][3], m2[2][3], m2[3][3]
		};
	}
	template <typename T>
	constexpr Matrix4<T> Matrix4<T>::inverse() const
	{
		Matrix4<T> temp = *this;
		return temp.invert();
	}


} // namespace els
