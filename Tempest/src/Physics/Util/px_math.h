#pragma once
#include "px_include.h"
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

	physx::PxMat44 to_physx_mat44(const mat4& mat4)
	{
		physx::PxMat44 newMat;

		newMat[0][0] = mat4[0][0];
		newMat[0][1] = mat4[0][1];
		newMat[0][2] = mat4[0][2];
		newMat[0][3] = mat4[0][3];

		newMat[1][0] = mat4[1][0];
		newMat[1][1] = mat4[1][1];
		newMat[1][2] = mat4[1][2];
		newMat[1][3] = mat4[1][3];

		newMat[2][0] = mat4[2][0];
		newMat[2][1] = mat4[2][1];
		newMat[2][2] = mat4[2][2];
		newMat[2][3] = mat4[2][3];

		newMat[3][0] = mat4[3][0];
		newMat[3][1] = mat4[3][1];
		newMat[3][2] = mat4[3][2];
		newMat[3][3] = mat4[3][3];


		return newMat;
	}

	mat4 to_mat4(const physx::PxMat44& mat )
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
}