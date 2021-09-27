#pragma once
#include "Graphics/OpenGL/Camera.h"
#include "Graphics/Basics/Mesh.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/Basics/Model.h"
#include "Util.h"

namespace Tempest
{
	enum struct ShaderCode
	{
		BASIC,
		FRAMEBUFFER,
		LINE,
		TEXTURE,
		GROUND,
		DIRECTIONAL_LIGHT
	};

	struct SpriteObj
	{
		glm::mat4 m_Transform;
		glm::mat3 m_Normal;
	};

	struct AAGrid
	{
		Mesh m_Mesh = GeometryFactory::GenerateIndexedPlane();
	};

	struct SpriteMesh
	{
		DrawElementsIndirect m_Indirect;
		VertexBuffer m_Instanced;
	};

	struct RenderPipeline
	{
		using ShaderLibrary = tmap<ShaderCode, tuptr<Shader>>;
		using MeshLibrary = tmap<MeshCode, tuptr<Mesh>>;

		/*
		*	Models
		*/
		tvector<tsptr<Model>> m_Models;
		tvector<glm::mat4> m_ModelTransforms;
		
		/*
		*	Polygons
		*/
		tvector<SpriteObj> m_Spheres;
		tvector<SpriteObj> m_Cubes;
		tvector<SpriteObj> m_Planes;
		tvector<SpriteObj> m_Icosahedrons;
		
		tvector<Camera> m_Cameras;
		ShaderLibrary m_Shaders;
		MeshLibrary m_Meshes;

		AAGrid Grid;
		SpriteMesh s_Mesh;
		VertexBuffer m_Indirect;
	};
}