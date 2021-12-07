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

#ifndef GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_SILENT_WARNINGS
#endif

#include "Math/elsCompare.h"
#include "Math/elsHeader.h"
#include "Math/elsMath.h"
#include "Math/elsMatrix3.h"
#include "Math/elsMatrix4.h"
#include "Math/elsVector3.h"
#include "Math/elsQuaternion.h"
#include "Math/elsRandom.h"
#include "Math/elsVectorGeneric.h"
#include "Math/elsNoise.h"
#include "Math/elsValue.h"

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp>
#include <gtx/vector_angle.hpp>
#include <gtx/easing.hpp>
#include <gtx/intersect.hpp> 


namespace Tempest
{
	using vec2 = glm::vec2;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;
	using mat3 = glm::mat3;
	using mat4 = glm::mat4;
	using quat = glm::quat;


	/*template <typename T = float>
	constexpr T to_rad(T degree)
	{
		return els::deg_to_rad(degree);
	}

	template <typename T = float>
	constexpr T to_deg(T radians)
	{
		return els::rad_to_deg(radians);
	}

	template <>
	constexpr vec3 to_rad<vec3>(vec3 degree)
	{
		return vec3{
			els::deg_to_rad(degree.x),
			els::deg_to_rad(degree.y),
			els::deg_to_rad(degree.z)
		};
	}

	template <>
	constexpr vec3 to_deg<vec3>(vec3 radians)
	{
		return vec3{
			els::rad_to_deg(radians.x),
			els::rad_to_deg(radians.y),
			els::rad_to_deg(radians.z)
		};
	}*/

	/*template <typename T>
	glm::vec3 to_glvec2(const T& v)
	{
		return glm::make_vec2(v.data());
	}
	template <typename T>
	glm::vec3 to_glvec3(const T& v)
	{
		return glm::make_vec3(v.data());
	}
	template <typename T>
	glm::vec4 to_glvec4(const T& v)
	{
		return glm::make_vec4(v.data());
	}
	template <typename T>
	glm::mat3 to_glmat3(const T& v)
	{
		return glm::make_mat3(v.data());
	}
	template <typename T>
	glm::mat4 to_glmat4(const T& v)
	{
		return glm::make_mat4(v.data());
	}
	template <typename T>
	glm::quat to_glquat(const T& v)
	{
		return glm::make_quat(v.data());
	}*/


	template <typename Archive>
	constexpr Archive& operator& (Archive& ar, vec2& vec2)
	{
		ar.StartArray();
		ar& vec2.x;
		ar& vec2.y;
		return ar.EndArray();
	}

	template <typename Archive>
	constexpr Archive& operator& (Archive& ar, vec3& vec3)
	{
		ar.StartArray();
		ar& vec3.x;
		ar& vec3.y;
		ar& vec3.z;
		return ar.EndArray();
	}

	template <typename Archive>
	constexpr Archive& operator& (Archive& ar, vec4& vec4)
	{
		ar.StartArray();
		ar& vec4.x;
		ar& vec4.y;
		ar& vec4.z;
		ar& vec4.w;
		return ar.EndArray();
	}
	template <typename Archive>
	constexpr Archive& operator& (Archive& ar, quat& quat)
	{
		ar.StartArray();
		ar& quat.x;
		ar& quat.y;
		ar& quat.z;
		ar& quat.w;
		return ar.EndArray();
	}

}