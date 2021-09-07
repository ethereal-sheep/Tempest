#pragma once
#include <glew.h>
#include "glm.hpp"
#include "vec3.hpp"
#include "Graphics/Basics/ShadowMap.h"
#include "TMath.h"

namespace Tempest
{
	enum struct LIGHTS
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOTLIGHT
	};

	struct Light
	{
		glm::vec3 Color = glm::vec3(1.f, 1.f, 1.f);
		float Intensity = 0.5f;
	};

	struct Directional_Light : public Light
	{
		glm::vec3 Direction = glm::vec3(0.f, 1.f, 0.f);
		ShadowMap DepthMap;
	};

	struct Point_Light : public Light
	{
		glm::vec3 Position;
	};

	struct SpotLight : public Light
	{
		glm::vec3 Position;
		glm::vec3 Min_Cutoff;
		glm::vec3 Max_Cutoff;
	};

	constexpr static Light GetLight(LIGHTS l)
	{
		switch (l)
		{
		case LIGHTS::DIRECTIONAL_LIGHT:	return Directional_Light{};
		case LIGHTS::POINT_LIGHT:		return Point_Light{};
		case LIGHTS::SPOTLIGHT:			return SpotLight{};
		}
	}
}