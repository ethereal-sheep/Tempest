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
#include "px_math.h"
#include "TMath.h"

namespace Tempest
{
	physx::PxVec3 to_physx_vec3(const vec3& vec)
	{
		return physx::PxVec3(vec.x, vec.y, vec.z);
	}

	physx::PxQuat to_physx_quat(const quat& qt)
	{
		return physx::PxQuat(qt.x, qt.y, qt.z, qt.w);
	}

	vec3 to_vec3(const physx::PxVec3& vec)
	{
		return vec3(vec.x, vec.y, vec.z);
	}

	quat to_quat(const physx::PxQuat& qt)
	{
		return quat(qt.w, qt.x, qt.y, qt.z);
	}

	physx::PxMat44 to_physx_mat44(const mat4& mat)
	{
		physx::PxMat44 newMat;

		newMat[0][0] = mat[0][0];
		newMat[0][1] = mat[0][1];
		newMat[0][2] = mat[0][2];
		newMat[0][3] = mat[0][3];

		newMat[1][0] = mat[1][0];
		newMat[1][1] = mat[1][1];
		newMat[1][2] = mat[1][2];
		newMat[1][3] = mat[1][3];

		newMat[2][0] = mat[2][0];
		newMat[2][1] = mat[2][1];
		newMat[2][2] = mat[2][2];
		newMat[2][3] = mat[2][3];

		newMat[3][0] = mat[3][0];
		newMat[3][1] = mat[3][1];
		newMat[3][2] = mat[3][2];
		newMat[3][3] = mat[3][3];


		return newMat;
	}

	mat4 to_mat4(const physx::PxMat44& mat)
	{
		mat4 newMat;

		newMat[0][0] = mat[0][0];
		newMat[0][1] = mat[0][1];
		newMat[0][2] = mat[0][2];
		newMat[0][3] = mat[0][3];

		newMat[1][0] = mat[1][0];
		newMat[1][1] = mat[1][1];
		newMat[1][2] = mat[1][2];
		newMat[1][3] = mat[1][3];

		newMat[2][0] = mat[2][0];
		newMat[2][1] = mat[2][1];
		newMat[2][2] = mat[2][2];
		newMat[2][3] = mat[2][3];

		newMat[3][0] = mat[3][0];
		newMat[3][1] = mat[3][1];
		newMat[3][2] = mat[3][2];
		newMat[3][3] = mat[3][3];

		return newMat;

	}

	vec3& math_cast(physx::PxVec3& vec) { return reinterpret_cast<vec3&>(vec); }
	physx::PxVec3& math_cast(vec3& vec) { return reinterpret_cast<physx::PxVec3&>(vec); }
	const vec3& math_cast(const physx::PxVec3& vec) { return reinterpret_cast<const vec3&>(vec); }
	const physx::PxVec3& math_cast(const vec3& vec) { return reinterpret_cast<const physx::PxVec3&>(vec); }

	quat& math_cast(physx::PxQuat& vec) { return reinterpret_cast<quat&>(vec); }
	physx::PxQuat& math_cast(quat& vec) { return reinterpret_cast<physx::PxQuat&>(vec); }
	const quat& math_cast(const physx::PxQuat& vec) { return reinterpret_cast<const quat&>(vec); }
	const physx::PxQuat& math_cast(const quat& vec) { return reinterpret_cast<const physx::PxQuat&>(vec); }

	mat4& math_cast(physx::PxMat44& mat) { return reinterpret_cast<mat4&>(mat); }
	physx::PxMat44& math_cast(mat4& mat) { return reinterpret_cast<physx::PxMat44&>(mat); }
	const mat4& math_cast(const physx::PxMat44& mat) { return reinterpret_cast<const mat4&>(mat); }
	const physx::PxMat44& math_cast(const mat4& mat) { return reinterpret_cast<const physx::PxMat44&>(mat); }


}