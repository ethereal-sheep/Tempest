#pragma once
#include "Graphics/OpenGL/Camera.h"
#include "Graphics/Basics/Mesh.h"
#include "Graphics/OpenGL/Shader.h"
#include "Util.h"

namespace Tempest
{
	enum struct ShaderCode
	{
		BASIC,
		FRAMEBUFFER,
		LINE
	};

	struct RenderPipeline
	{
		using ShaderLibrary = tmap<ShaderCode, tuptr<Shader>>;
		using MeshLibrary = tmap<MeshCode, tuptr<Mesh>>;

		tvector<Camera> m_Cameras;
		tvector<glm::mat4> m_Transforms;
		tvector<MeshCode> m_Sprites;
		ShaderLibrary m_Shaders;
		MeshLibrary m_Meshes;
	};
}