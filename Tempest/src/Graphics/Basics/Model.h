#pragma once
#include "Graphics/Basics/Material.h"
#include "Util.h"

namespace Tempest
{
	class Model
	{
		struct MeshData
		{
			tvector<vec3> position;
			tvector<vec3> normal;
			tvector<vec3> texcoords;
			tvector<uint32_t> indices;
			uint32_t materialIndex;
		};

		Material mMaterial;

	public:

		Model() = default;
		~Model() = default;

		const aiScene* g_scene = nullptr;
		Assimp::Importer importer;


		void LoadModel(const char* file)
		{
			m_filepath = file;

			CDN_CORE_WARN("Loading model: {0}", m_filepath);

			m_pScene = m_importer.ReadFile(m_filepath,
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_JoinIdenticalVertices);

			LOG_ASSERT(m_pScene && m_pScene->mRootNode && !(m_pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE),
				m_importer.GetErrorString());

			/*
			*	Process Object Node Data
			*/

			/*
			*	Process Object Mesh Data { Position, Normal, Tex-Coords, ... }
			*/

			/*
			*	Process Object Material Textures { Diffuse Maps, Specular Maps, Emissive Map, ... }
			*/

			/*
			*	Process Material Info.. { Diffuse, Specular, Reflectiveness, ... }
			*/

		}

		void ProcessNodeData();		// Nodes
		void ProcessMeshData();		// Mesh Data
		void ProcessMaterialData(); // Material Textures + Material Info

	};
}