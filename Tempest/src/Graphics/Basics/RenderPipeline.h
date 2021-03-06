/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
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
#include "Graphics/PBR/MeshPrimitives.h"

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
		saoBlurShader,

		bloomBlurShader
			
	};

	struct SpriteObj
	{
		glm::mat4 m_Transform;
		glm::mat3 m_Normal;
	};

	struct ModelObj
	{
		tsptr<ModelPBR> m_Model;
		glm::mat4 m_Transform{ 1.0f };
		glm::mat4 m_TransformPrev{ 1.0f };
		bool hasColor = false;
		bool isAnim = false;
		vec3 color = { 0.f, 0.f, 0.f };
		bool isParticle = false;
		bool m_Emissive = false;
		bool normalCalculated = false;
		tvector<glm::mat4> m_Bones;
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

	struct ParticleObj
	{
		glm::mat4 m_Transform;
		glm::mat3 m_Normal;
	};

	struct ParticlePlane
	{
		ParticleObj m_Object;
		MeshPrim m_Mesh = GeometryFactory::GenerateIndexedPlane();
	};

	struct ParticleCube
	{
		ParticleObj m_Object;
		MeshPrim m_Mesh = GeometryFactory::GenerateIndexedCube(1, 1);
	};

	struct ParticleSphere
	{
		ParticleObj m_Object;
		MeshPrim m_Mesh = GeometryFactory::GenerateIndexedSphere(1, 1);
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

		tvector<ParticlePlane> p_Plane;
		tvector<ParticleCube> p_Cube;
		tvector<ParticleSphere> p_Sphere;
		
		tvector<Camera> m_Cameras;
		ShaderLibrary m_Shaders;
		MeshLibrary m_Meshes;

		AAGrid Grid;
		SpriteMesh s_Mesh;
		VertexBuffer m_Indirect;
	};
}