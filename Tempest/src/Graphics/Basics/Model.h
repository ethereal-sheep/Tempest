#pragma once
#include "Graphics/Basics/Material.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Graphics/Basics/Mesh.h"
#include "Util.h"
#include "TMath.h"
#include "Util/Range.h"

namespace Tempest
{
	class Mesh;
	struct Material;

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

		//Material mMaterial;

	public:

		Model(const char* file);
		Model(Model&& model);
		Model& operator=(Model&& model) noexcept;
		~Model() = default;

		auto GetMeshes() const
		{
			return make_const_range(m_Meshes);
		}

	private:
		void ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform);		// Nodes
		void ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform);		// Mesh Data
		void ProcessMaterialData(); // Material Textures + Material Info

		tpath m_File;
		tvector<Material> m_Materials;
		tvector<tpair<Mesh, Material*>> m_Meshes;

	};
}