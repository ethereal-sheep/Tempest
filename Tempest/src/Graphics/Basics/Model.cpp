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
#include "TMath.h"

namespace Tempest
{
	thread_local Assimp::Importer s_Importer;
	thread_local const aiScene* s_Scene;


	Model::Model(const char* file) : m_File(file)
	{
		//s_Scene = s_Importer.ReadFile(file,
		//	aiProcess_Triangulate |
		//	aiProcess_GenSmoothNormals |
		//	aiProcess_JoinIdenticalVertices);
		//
		//// create a list of material
		//tvector<tsptr<Material>> materials;
		////auto tex = s_Scene->GetEmbeddedTexture(m_File.string().c_str());
		//
		//// pass in materials to be processed
		//for (id_t i = 0; i < s_Scene->mNumMaterials; ++i)
		//{
		//	Material m;
		//	aiString atex;
		//	const auto* pMaterial = s_Scene->mMaterials[i];
		//	// turn into Tempest::Material
		//	//auto& m = *materials[i];
		//	pMaterial->Get(AI_MATKEY_REFRACTI, m.Refraction);
		//	pMaterial->Get(AI_MATKEY_REFLECTIVITY, m.Reflection);
		//	pMaterial->Get(AI_MATKEY_SHININESS, m.Shininess);
		//	pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, m.ShininessStrength);
		//	pMaterial->Get(AI_MATKEY_OPACITY, m.Opacity);
		//
		//	pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, m.Diffuse.data(), nullptr);
		//	pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, m.Ambient.data(), nullptr);
		//	pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, m.Specular.data(), nullptr);
		//	pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, m.Emissive.data(), nullptr);
		//	pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, m.Transparent.data(), nullptr);
		//	pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, m.Reflective.data(), nullptr);
		//	pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, i), atex);
		//	if (atex.length)
		//	{
		//		string full_path{ atex.data };
		//		string lower_path{ atex.data };
		//		std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(),
		//			[](char c) { return (char)std::tolower((int)c); });
		//
		//		auto check = lower_path.find("models");
		//		if (check == string::npos) continue;
		//
		//		string tex_path = full_path.substr(check, full_path.length());
		//		//auto file2 = m_File.parent_path() / tex_path;
		//
		//		try
		//		{
		//			m.BaseTexture = make_sptr<Texture>(tex_path);
		//		}
		//		catch (const std::exception& e)
		//		{
		//			LOG_ERROR(e.what());
		//		}
		//	}
		//	//else
		//	//{
		//	//	m.BaseTexture = nullptr;
		//	//}
		//
		//	for (uint32_t j = 0; j < pMaterial->GetTextureCount(aiTextureType_DIFFUSE); ++j)
		//	{
		//		aiString path;
		//		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		//		{
		//			string full_path{ path.data };
		//			string lower_path{ path.data };
		//			std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(),
		//				[](char c) { return (char)std::tolower((int)c); });
		//
		//			auto check = lower_path.find("textures");
		//			if (check == string::npos) continue;
		//
		//			string tex_path = full_path.substr(check, full_path.length());
		//			auto tfile = m_File.parent_path() / tex_path;
		//
		//			try
		//			{
		//				m.DiffuseMap = make_sptr<Texture>(tfile.string().c_str());
		//			}
		//			catch (const std::exception& e)
		//			{
		//				LOG_ERROR(e.what());
		//			}
		//
		//			//auto tex_path = m_File.parent_path() / path.data;
		//			//m.DiffuseMap = make_sptr<Texture>(tex.path);
		//		}
		//	}
		//	materials.push_back(make_sptr<Material>(m));
		//}
		//
		//// nodes and meshes
		//// recursive into the scene graph
		//ProcessNodeData(s_Scene->mRootNode, aiMatrix4x4{}, materials);

		std::vector<glm::vec3> pos;
		std::vector<glm::vec3> norm;
		std::vector<glm::vec2> tex;
		std::vector<glm::ivec3> faces;
		std::vector<std::string> textures;
		std::vector<uint32_t> sizes;
		std::vector<uint32_t> sides;
		std::vector<uint32_t> mats;
		std::vector<Material> mm;
		Material m;

		std::string temp_x, temp_y, temp_z;

		std::ifstream in_file{ file, std::ios::in };
		std::string line;
		std::string prefix;

		// Check if file was opened properly, else return false
		if (!in_file)
		{
			LOG_WARN("File Not Found");
		}

		in_file.seekg(0, std::ios::beg);

		while (std::getline(in_file, line))
		{
			std::istringstream file_line{ line };
			glm::vec3 r;
			file_line >> prefix;
			if (prefix == "v" && line != "")
			{
				file_line >> r.x >> r.y >> r.z;

				pos.push_back(r);
			}

			else if (prefix == "n" && line != "")
			{
				glm::vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;

				norm.push_back(temp);
			}

			else if (prefix == "t" && line != "")
			{
				glm::vec2 temp;
				file_line >> temp.x >> temp.y;

				tex.push_back(temp);
			}

			else if (prefix == "f" && line != "")
			{
				glm::ivec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;

				faces.push_back(temp);
			}

			else if (prefix == "p" && line != "")
			{
				std::string temp;
				file_line >> temp;
				std::filesystem::path parent{ file };
				std::string p = parent.parent_path().string();
				p.erase(p.end() - 6, p.end());
				p += temp;
				try
				{
					m.BaseTexture = make_sptr<Texture>(p);
				}

				catch (...)
				{
					LOG_WARN("Texture File Not Found!");
				}
				
			}

			else if (prefix == "m" && line != "")
			{
				unsigned int temp;
				file_line >> temp;

				sizes.push_back(temp);

			}

			else if (prefix == "w" && line != "")
			{
				unsigned int temp;
				file_line >> temp;

				sides.push_back(temp);

			}

			else if (prefix == "g" && line != "")
			{
				unsigned int temp;
				file_line >> temp;

				mats.push_back(temp);

			}

			else if (prefix == "Ambient" && line != "")
			{
				vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;
				m.Ambient = temp;
			}

			else if (prefix == "Diffuse" && line != "")
			{
				vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;
				m.Diffuse = temp;
			}

			else if (prefix == "Specular" && line != "")
			{
				vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;
				m.Specular = temp;
			}

			else if (prefix == "Emissive" && line != "")
			{
				vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;
				m.Emissive = temp;
			}

			else if (prefix == "Transparent" && line != "")
			{
				vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;
				m.Transparent = temp;
			}

			else if (prefix == "Reflective" && line != "")
			{
				vec3 temp;
				file_line >> temp.x >> temp.y >> temp.z;
				m.Reflective = temp;
			}

			else if (prefix == "Refraction" && line != "")
			{
				float temp;
				file_line >> temp;
				m.Refraction = temp;
			}

			else if (prefix == "Reflection" && line != "")
			{
				float temp;
				file_line >> temp;
				m.Reflection = temp;
			}

			else if (prefix == "Shininess" && line != "")
			{
				float temp;
				file_line >> temp;
				m.Shininess = temp;
			}

			else if (prefix == "ShininessStrength" && line != "")
			{
				float temp;
				file_line >> temp;
				m.ShininessStrength = temp;
			}

			else if (prefix == "Opacity" && line != "")
			{
				float temp;
				file_line >> temp;
				m.Opacity = temp;
			}
			//glm::vec3 n = normalize(pos);
			//glm::vec3 c1 = cross(n, glm::vec3(0.0, 0.0, 1.0));
			//glm::vec3 c2 = cross(n, glm::vec3(0.0, 1.0, 0.0));
			//
			//glm::vec3 t = normalize(length(c1) > length(c2) ? c1 : c2);
			//glm::vec3 b = cross(t, n);
			//
			//vertex.tangent.emplace_back(t);
			//vertex.bitangent.emplace_back(b);
		}
		mm.push_back(m);

		int count = 0;
		int joints = 0;
		for (int i = 0; i < sizes.size(); ++i)
		{
			tpair<Vertices, Indices> vi;
			Vertices& vertex = vi.first;
			Indices& index = vi.second;

			int ind = sizes[i] + count;
			for (int j = count; j < ind; ++j)
			{
				vertex.position.push_back(pos[j]);
				vertex.normal.push_back(norm[j]);
				vertex.texCoord.push_back(tex[j]);
				glm::vec3 n = normalize(pos[j]);
				glm::vec3 c1 = cross(n, glm::vec3(0.0, 0.0, 1.0));
				glm::vec3 c2 = cross(n, glm::vec3(0.0, 1.0, 0.0));
				
				glm::vec3 t = normalize(length(c1) > length(c2) ? c1 : c2);
				glm::vec3 b = cross(t, n);
				
				vertex.tangent.emplace_back(t);
				vertex.bitangent.emplace_back(b);
			}
			 
			for (int j = joints; j < joints + sides[i]; ++j)
			{
				index.push_back(faces[j].x);
				index.push_back(faces[j].y);
				index.push_back(faces[j].z);
			}

			joints += sides[i];
			count += sizes[i];

			if (mats[i] >= mm.size())
			{
				Material t;
				t.BaseTexture = nullptr;
				m_Meshes.emplace_back(std::move(vi), make_sptr<Material>(t));
			}

			else
				m_Meshes.emplace_back(std::move(vi), make_sptr<Material>(m));
		}
		//m_Meshes.emplace_back(std::move(vi), make_sptr<Material>(m));
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