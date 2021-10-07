/**********************************************************************************
* \author		_ (_@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Graphics/Basics/Model.h"
#include "Logger/Log.h"
#include "Core.h"

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

		// create a list of material
		tvector<tsptr<Material>> materials;
		//auto tex = s_Scene->GetEmbeddedTexture(m_File.string().c_str());

		// pass in materials to be processed
		for (id_t i = 0; i < s_Scene->mNumMaterials; ++i)
		{
			Material m;
			aiString atex;
			const auto* pMaterial = s_Scene->mMaterials[i];
			// turn into Tempest::Material
			//auto& m = *materials[i];
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
			pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, i), atex);
			if (atex.length)
			{
				string full_path{ atex.data };
				string lower_path{ atex.data };
				std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(),
					[](char c) { return (char)std::tolower((int)c); });

				auto check = lower_path.find("models");
				if (check == string::npos) continue;

				string tex_path = full_path.substr(check, full_path.length());
				//auto file2 = m_File.parent_path() / tex_path;

				try
				{
					m.BaseTexture = make_sptr<Texture>(tex_path);
				}
				catch (const std::exception& e)
				{
					LOG_ERROR(e.what());
				}
			}
			//else
			//{
			//	m.BaseTexture = nullptr;
			//}

			for (uint32_t j = 0; j < pMaterial->GetTextureCount(aiTextureType_DIFFUSE); ++j)
			{
				aiString path;
				if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
				{
					string full_path{ path.data };
					string lower_path{ path.data };
					std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(),
						[](char c) { return (char)std::tolower((int)c); });

					auto check = lower_path.find("textures");
					if (check == string::npos) continue;

					string tex_path = full_path.substr(check, full_path.length());
					auto tfile = m_File.parent_path() / tex_path;

					try
					{
						m.DiffuseMap = make_sptr<Texture>(tfile.string().c_str());
					}
					catch (const std::exception& e)
					{
						LOG_ERROR(e.what());
					}

					//auto tex_path = m_File.parent_path() / path.data;
					//m.DiffuseMap = make_sptr<Texture>(tex.path);
				}
			}
			materials.push_back(make_sptr<Material>(m));
		}

		// nodes and meshes
		// recursive into the scene graph
		ProcessNodeData(s_Scene->mRootNode, aiMatrix4x4{}, materials);
	}

	/*Model::Model(Model&& model)
	:	m_File{std::move(model.m_File)},
		m_Materials{std::move(model.m_Materials)},
		m_Meshes{std::move(model.m_Meshes)}
	{
	}

	Model& Model::operator=(Model&& model) noexcept
	{
		std::swap(m_File, model.m_File);
		std::swap(m_Materials, model.m_Materials);
		std::swap(m_Meshes, model.m_Meshes);

		return *this;
	}*/

	void Model::ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform, const tvector<tsptr<Material>>& materials)
	{
		for (id_t i = 0; i < node->mNumMeshes; ++i)
			ProcessMeshData(s_Scene->mMeshes[node->mMeshes[i]], transform * node->mTransformation, materials);
		
		// process all of its children's node meshes (if any)
		for (id_t i = 0; i < node->mNumChildren; ++i)
		{
			ProcessNodeData(node->mChildren[i], transform * node->mTransformation, materials);
		}
	}

	void Model::ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform, const tvector<tsptr<Material>>& materials)
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
			vertex.position.emplace_back(glm::vec3(els::to_vec3(transform * (*pPos))));
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
			tpair<Mesh, tsptr<Material>>(
				std::move(vi), 
				materials[mesh->mMaterialIndex])
			);
	}

	//void Model::ProcessMaterialData()
	//{
	//	//m_Materials.resize(s_Scene->mNumMaterials);
	//	tvector<Material> materials(s_Scene->mNumMaterials);

	//	
	//}
}