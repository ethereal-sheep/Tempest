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
#include "px_include.h"
#include "TMath.h"

namespace Tempest
{
	physx::PxVec3 to_physx_vec3(const vec3& vec);
	physx::PxQuat to_physx_quat(const quat& qt);
	physx::PxMat44 to_physx_mat44(const mat4& mat);

	vec3 to_vec3(const physx::PxVec3& vec);
	quat to_quat(const physx::PxQuat& qt);
	mat4 to_mat4(const physx::PxMat44& mat);

	vec3& math_cast(physx::PxVec3& vec);
	physx::PxVec3& math_cast(vec3& vec);
	const vec3& math_cast(const physx::PxVec3& vec);
	const physx::PxVec3& math_cast(const vec3& vec);

	quat& math_cast(physx::PxQuat& vec);
	physx::PxQuat& math_cast(quat& vec);
	const quat& math_cast(const physx::PxQuat& vec);
	const physx::PxQuat& math_cast(const quat& vec);

	mat4& math_cast(physx::PxMat44& mat);
	physx::PxMat44& math_cast(mat4& mat);
	const mat4& math_cast(const physx::PxMat44& mat);
	const physx::PxMat44& math_cast(const mat4& mat);
}