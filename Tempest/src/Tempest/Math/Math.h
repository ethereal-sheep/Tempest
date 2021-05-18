#pragma once

#include "els/elsCompare.h"
#include "els/elsHeader.h"
#include "els/elsMath.h"
#include "els/elsMatrix3.h"
#include "els/elsMatrix4.h"
#include "els/elsVector3.h"
#include "els/elsQuaternion.h"
#include "els/elsRandom.h"
#include "els/elsVectorGeneric.h"
#include "els/elsNoise.h"


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