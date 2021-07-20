#include "Graphics/Basics/Model.h"
#include "Logger/Log.h"

namespace Tempest
{
	thread_local Assimp::Importer s_Importer;
	thread_local const aiScene* s_Scene;


	Model::Model(const char* file) : m_File(file)
	{
		s_Scene = s_Importer.ReadFile(file,
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices);

		// materials
		ProcessMaterialData();

		// nodes and meshes
		ProcessNodeData(s_Scene->mRootNode);
	}

	void Model::ProcessNodeData(const aiNode* node)
	{
		for (id_t i = 0; i < node->mNumMeshes; ++i)
			ProcessMeshData(s_Scene->mMeshes[node->mMeshes[i]]);
		
		// process all of its children's node meshes (if any)
		for (id_t i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNodeData(node->mChildren[i]);
		}
	}

	void Model::ProcessMeshData(const aiMesh* mesh)
	{
		tpair<Vertices, Indices> vi;
		Vertices& vertex = vi.first;
		Indices& index = vi.second;

		const aiVector3D zero3D(0.f, 0.f, 0.f);
		for (size_t i = 0; i < mesh->mNumVertices; ++i)
		{
			const aiVector3D* pPos = &(mesh->mVertices[i]);
			const aiVector3D* pNormal = &(mesh->mNormals[i]);
			const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero3D;

			// push back vertex attributes
			vertex.position.emplace_back(glm::vec3(els::to_vec3(*pPos)));
			vertex.normal.emplace_back(glm::vec3(els::to_vec3(*pNormal)));
			vertex.texCoord.emplace_back(pTexCoord->x, pTexCoord->y);

			glm::vec3 n = normalize(glm::vec3(els::to_vec3(*pPos)));
			glm::vec3 c1 = cross(n, glm::vec3(0.0, 0.0, 1.0));
			glm::vec3 c2 = cross(n, glm::vec3(0.0, 1.0, 0.0));
			
			glm::vec3 t = normalize(length(c1) > length(c2) ? c1 : c2);
			glm::vec3 b = cross(t, n);

			vertex.tangent.emplace_back(t);
			vertex.bitangent.emplace_back(b);
		}

		for (size_t i = 0; i < mesh->mNumFaces; ++i)
		{
			const aiFace& face = mesh->mFaces[i];

			// push back indices
			index.emplace_back(face.mIndices[0]);
			index.emplace_back(face.mIndices[1]);
			index.emplace_back(face.mIndices[2]);
		}

		m_Meshes.emplace_back(
			tpair<Mesh, Material*>(
				std::move(vi), 
				&m_Materials[mesh->mMaterialIndex])
			);
	}

	void Model::ProcessMaterialData()
	{
		m_Materials.resize(s_Scene->mNumMaterials);
			
		for (id_t i = 0; i < s_Scene->mNumMaterials; ++i)
		{
			const auto* pMaterial = s_Scene->mMaterials[i];
			// turn into Tempest::Material
			auto& m = m_Materials[i];
			pMaterial->Get(AI_MATKEY_REFRACTI, m.Refraction);
			pMaterial->Get(AI_MATKEY_REFLECTIVITY, m.Reflection);
			pMaterial->Get(AI_MATKEY_SHININESS, m.Shininess);
			pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, m.ShininessStrength);
			pMaterial->Get(AI_MATKEY_OPACITY, m.Opacity);
			
			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, m.Diffuse.data(), nullptr);
			pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, m.Ambient.data(), nullptr);
			pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, m.Specular.data(), nullptr);
			pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, m.Emissive.data(), nullptr);
			pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, m.Transparent.data(), nullptr);
			pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, m.Reflective.data(), nullptr);

		}
	}
}