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

#include "elsVector2.h"
#include "elsVector3.h"
#include "elsVector4.h"
#include "elsMatrix3.h"
#include "elsMatrix4.h"


namespace els
{
	template <typename TData>
	constexpr typename TData::Scalar const* value_ptr(const TData& x)
	{
		return x.data();
	}
	template <typename TData>
	constexpr typename TData::Scalar* value_ptr(TData& x)
	{
		return x.data();
	}

	template <typename T>
	constexpr Vector2<T> make_vec2(T const* ptr)
	{
		return Vector2<T>{*ptr, * (++ptr)};
	}

	template <typename T>
	constexpr Vector3<T> make_vec3(T const* ptr)
	{
		return Vector3<T>{*ptr, * (++ptr), * (++ptr)};
	}

	template <typename T>
	constexpr Vector4<T> make_vec4(T const* ptr)
	{
		return Vector4<T>{*ptr, * (++ptr), * (++ptr), * (++ptr)};
	}

	template <typename T>
	constexpr Matrix3<T> make_mat3(T const* ptr)
	{
		return Matrix3<T>{*ptr, * (++ptr), * (++ptr),
			* (++ptr), * (++ptr), * (++ptr),
			* (++ptr), * (++ptr), * (++ptr)};
	}
	
	template <typename T>
	constexpr Matrix4<T> make_mat4(T const* ptr)
	{
		return Matrix4<T>{*ptr, * (++ptr), * (++ptr), * (++ptr), 
			* (++ptr), * (++ptr), * (++ptr), * (++ptr), 
			* (++ptr), * (++ptr), * (++ptr), * (++ptr), 
			* (++ptr), * (++ptr), * (++ptr), * (++ptr)};
	}

}
