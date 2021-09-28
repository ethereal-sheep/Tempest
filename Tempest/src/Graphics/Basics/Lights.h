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

	struct Directional_Light
	{
		//ShadowMap DepthMap;		// Depth Map(Texture) for Shadow Mapping
		//glm::mat4 ProjectionMatrix = glm::mat4(1.f);
		glm::vec3 Color = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 Direction = glm::vec3(0.f, 1.f, 0.f);
		float Intensity = 0.5f;
	};

	struct Point_Light
	{
		//ShadowMap DepthMap;		// Depth Map(Texture) for Shadow Mapping
		//glm::mat4 ProjectionMatrix[6];
		glm::vec3 Color = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 Position = glm::vec3(0.f, 0.f, 1.f);
		float Radius = 0.5f;
		float Intensity = 0.7f;
	};

	struct SpotLight
	{
		//ShadowMap DepthMap;		// Depth Map(Texture) for Shadow Mapping
		//glm::mat4 ProjectionMatrix = glm::mat4(1.f);
		glm::vec3 Color = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 Position = glm::vec3(0.f, 0.f, 1.f);
		float Max_Cutoff = 1.f;		// Outer Cutoff
		float Min_Cutoff = 0.1f;	// Inner Cutoff
		float Intensity = 0.5f;
	};
}