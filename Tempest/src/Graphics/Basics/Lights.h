#pragma once
#include <glew.h>
#include "glm.hpp"
#include "vec3.hpp"
#include "Graphics/Basics/ShadowMap.h"
#include "TMath.h"

namespace Tempest
{
	// Shadow map buffer size width x height
	const uint32_t SHADOW_MAP_SIZE = 2048;

	enum struct LIGHTS
	{
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,
		SPOTLIGHT
	};

	struct Directional_Light
	{
		glm::vec3 Color = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 Direction = glm::vec3(0.f, 1.f, 0.0001f);
		float Intensity = 0.5f;
		uint32_t m_dirFBO = 0;
		uint32_t m_depthmap = 0;
		uint32_t m_Width = SHADOW_MAP_SIZE;
		uint32_t m_Height = SHADOW_MAP_SIZE;

		bool hide = false;

		Directional_Light(); // Make Directional Light FBO
		void Bind();
	};

	struct Point_Light
	{
		glm::vec3 Position = glm::vec3(0.f, 0.f, 1.f);
		//float Radius = 0.5f;
		float Intensity = 0.7f;
		
		glm::vec3 Color = glm::vec3(1.f, 1.f, 1.f);

		float pointLightConsts = 0.7f;
		float pointLightLinears = 0.15f; // or   0.09f
		float pointLightQuads = 0.052f; // or  0.032f;

		uint32_t m_pointFBO = 0;
		uint32_t m_cubemap = 0;
		uint32_t m_Width = SHADOW_MAP_SIZE;
		uint32_t m_Height = SHADOW_MAP_SIZE;

		bool hide = true;
		Point_Light(); // Make Point Light FBO
		void Bind();
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