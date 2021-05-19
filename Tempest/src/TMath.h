#pragma once

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


namespace Tempest
{
	template <typename T = float>
	constexpr T ToRadians(T degree)
	{
		return els::deg_to_rad(degree);
	}

	template <typename T = float>
	constexpr T ToDegrees(T radians)
	{
		return els::rad_to_deg(radians);
	}
}