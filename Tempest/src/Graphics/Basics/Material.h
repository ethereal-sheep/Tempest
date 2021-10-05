#pragma once
#include <glew.h>
#include "TMath.h"
#include "Util.h"
#include "Graphics/OpenGL/Texture.h"
#include <assimp/Importer.hpp>

namespace Tempest
{
	class Texture;

	struct Material
	{
		tsptr<Texture> BaseTexture = nullptr;
		tsptr<Texture> DiffuseMap = nullptr;
		tsptr<Texture> EmissiveMap = nullptr;
		tsptr<Texture> SpecularMap = nullptr;
		tsptr<Texture> NormalMap = nullptr;

		vec3 Diffuse = vec3(0.f, 0.f, 0.f);
		vec3 Ambient = vec3(0.f, 0.f, 0.f);
		vec3 Specular = vec3(0.f, 0.f, 0.f);
		vec3 Emissive = vec3(0.f, 0.f, 0.f);
		vec3 Transparent = vec3(0.f, 0.f, 0.f);
		vec3 Reflective = vec3(0.f, 0.f, 0.f);

		float Refraction = 0.f;
		float Reflection = 0.f;
		float Shininess = 0.f;
		float ShininessStrength = 0.f;
		float Opacity = 1.f;

	};
}