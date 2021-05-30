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

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp>


namespace Tempest
{
	using vec2 = els::vec2f;
	using vec3 = els::vec3f;
	using vec4 = els::vec4f;
	using mat3 = els::mat3f;
	using mat4 = els::mat4f;
	using quat = glm::quat;


	template <typename T = float>
	constexpr T to_rad(T degree)
	{
		return els::deg_to_rad(degree);
	}

	template <typename T = float>
	constexpr T to_deg(T radians)
	{
		return els::rad_to_deg(radians);
	}

	template <typename T>
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
	}

}