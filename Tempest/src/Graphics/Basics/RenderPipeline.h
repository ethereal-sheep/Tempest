/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "Graphics/OpenGL/Camera.h"
#include "Graphics/Basics/Mesh.h"
#include "Graphics/OpenGL/Shader.h"
#include "Graphics/Basics/Model.h"
#include "Util.h"
#include "Graphics/PBR/ModelPBR.h"

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
		MODEL_TEXTURE,
		MODEL_LIGHT

		, // PBR TEST
		gBufferShader,
		latlongToCubeShader,
		simpleShader,
		lightingBRDFShader,
		irradianceIBLShader,
		prefilterIBLShader,
		integrateIBLShader,

		firstpassPPShader,
		saoShader,
		saoBlurShader
			
	};

	struct SpriteObj
	{
		glm::mat4 m_Transform;
		glm::mat3 m_Normal;
	};

	struct ModelObj
	{
		tsptr<ModelPBR> m_Model;
		glm::mat4 m_Transform;
		glm::mat4 m_TransformPrev;
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
		tmap<string, tsptr<ModelPBR>> m_ModelLibrary;
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