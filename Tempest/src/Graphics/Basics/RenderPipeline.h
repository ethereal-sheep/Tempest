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
		LIGHTING,
		DIRECTIONAL_SHADOW_MAP,
		POINT_LIGHT_DEPTH,
		MODEL,
		MODEL_TEXTURE
	};

	struct SpriteObj
	{
		glm::mat4 m_Transform;
		glm::mat3 m_Normal;
	};

	struct ModelObj
	{
		tsptr<Model> m_Model;
		glm::mat4 m_Transform;
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
		tmap<string, tsptr<Model>> m_ModelLibrary;
		tvector<ModelObj> m_Models;
		
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