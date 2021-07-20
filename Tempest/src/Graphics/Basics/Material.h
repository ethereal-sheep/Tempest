#pragma once
#include <glew.h>
#include "TMath.h"
#include "Util.h"

namespace Tempest
{
	class Texture;

	struct Material
	{
		std::string m_ID;

		vec3 mDiffuse = vec3(0.f, 0.f, 0.f);
		vec3 mAmbient = vec3(0.f, 0.f, 0.f);
		vec3 mSpecular = vec3(0.f, 0.f, 0.f);
		vec3 mEmissive = vec3(0.f, 0.f, 0.f);
		vec3 mTransparent = vec3(0.f, 0.f, 0.f);
		vec3 mReflective = vec3(0.f, 0.f, 0.f);


		Texture* mDiffuseMap = nullptr;
		Texture* mEmissiveMap = nullptr;
		Texture* mSpecularMap = nullptr;
		Texture* mNormalMap = nullptr;

		float Refraction = 0.f;
		float Reflection = 0.f;
		float Shininess = 0.f;
		float Shininess_Strength = 0.f;
		float Transparency = 1.f;
		
		bool isVisible = true;
		bool castShadow = false;
	};
}