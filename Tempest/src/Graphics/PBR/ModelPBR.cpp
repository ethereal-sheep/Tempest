/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ModelPBR.h"
#include "Logger/Log.h"
#include <filesystem>
#include <cstring>

namespace Tempest
{
    ModelPBR::ModelPBR()
    {

    }


    ModelPBR::~ModelPBR()
    {

    }

	void ModelPBR::LoadTextures(const aiScene* scene)
	{
		for (auto i = 0; i < scene->mNumMaterials; ++i)
		{
			auto pMaterial = scene->mMaterials[i];
			aiString texture;
			TexturePBR tb;
			pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, i), texture);

			if (texture.length)
			{
				mm.push_back(tb);
			}
		}
	}

	void ModelPBR::LoadMaterial(const aiScene* scene)
	{
		for (auto i = 0; i < scene->mNumMaterials; ++i)
		{
			auto pMaterial = scene->mMaterials[i];
			aiColor3D diffuse;
			glm::vec3 dif;

			pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

			dif.x = diffuse.r;
			dif.y = diffuse.g;
			dif.z = diffuse.b;

			colours.push_back(dif);
		}
	}


    void ModelPBR::loadModel(std::string file)
    {
		std::filesystem::path asd{ file };
		//Assimp::Importer importer;
		//const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (strcmp(asd.extension().string().c_str(), ".a"))
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);

			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				return;
			}
			if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
			{
				std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
				return;
			}

			this->directory = file.substr(0, file.find_last_of('/'));
			this->processNode(scene->mRootNode, scene);
			//LoadMaterial(scene);

			// Multiple Animations embedded in 1 fbx
			//for (unsigned int i = 0; i < static_cast<unsigned int>(scene->mNumAnimations); ++i)
			//{
			//	std::string name{ scene->mAnimations[i]->mName.C_Str() };
			//	Animation anim(m_BoneInfoMap, m_BoneCounter, scene, i);
			//	animations.insert(std::make_pair(name, anim));
			//}

			m_Animation = Animation(m_BoneInfoMap, m_BoneCounter, scene, 0);	// Single Animation embedded in 1 file
		}

		else
		{
			std::vector<glm::vec3> pos;
			std::vector<glm::vec3> norm;
			std::vector<glm::vec2> tex;
			std::vector<glm::ivec3> faces;
			std::vector<std::string> textures;
			std::vector<uint32_t> sizes;
			std::vector<uint32_t> sides;

			std::string temp_x, temp_y, temp_z;

			std::ifstream in_file{ file, std::ios::in };
			std::string line;
			std::string prefix;

			// Check if file was opened properly, else return false
			if (!in_file)
			{
				std::string aa{ "File Not Found: " };
				std::stringstream ss;
				ss << aa;
				ss << file;
				LOG_WARN(ss.str());
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
					TexturePBR tb;
					std::string temp;
					file_line >> temp;
					if (!strcmp(temp.c_str(), "NULL"))
					{
						
						tb.setTexture("0", "0", true);
					}

					else
					{
						std::filesystem::path parent{ file };
						std::string p = parent.parent_path().string();
						p.erase(p.end() - 6, p.end());
						p += temp;
						std::filesystem::path ext{ temp };
						std::string xt = ext.extension().string();
						if (!strcmp(xt.c_str(), ".dds"))
						{
							try
							{
								//MaterialPBR m;
								tb.setTexturePath(p.c_str(), parent.filename().string(), true, TexturePBR::TextureFileType::DDS);
								//m.addTexture(parent.filename().string(), std::move(tex));
							}

							catch (...)
							{
								LOG_WARN("Texture File Not Found!");
							}
						}

						else if (!strcmp(xt.c_str(), ".png"))
						{
							try
							{
								//MaterialPBR m;
								tb.setTexturePath(p.c_str(), parent.filename().string(), true, TexturePBR::TextureFileType::STBImage);
								//m.addTexture(parent.filename().string(), std::move(tex));
							}

							catch (...)
							{
								LOG_WARN("Texture File Not Found!");
							}
						}
						
					}

					mm.push_back(tb);
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

				else if (prefix == "Diffuse" && line != "")
				{
					glm::vec3 q;
					file_line >> q.x >> q.y >> q.z;

					colours.push_back(q);
				}
			}
			//mm.push_back(m);

			int count = 0;
			int joints = 0;
			for (int i = 0; i < sizes.size(); ++i)
			{
				std::vector<Vertex> vertices;
				std::vector<GLuint> indices;

				int ind = sizes[i] + count;
				for (int j = count; j < ind; ++j)
				{
					Vertex vert;
					vert.Position = pos[j];
					vert.Normal = norm[j];
					vert.TexCoords = tex[j];

					vertices.push_back(vert);
				}

				for (int j = joints; j < joints + (int)(sides[i]); ++j)
				{
					indices.push_back(faces[j].x);
					indices.push_back(faces[j].y);
					indices.push_back(faces[j].z);
				}

				joints += sides[i];
				count += sizes[i];

				MeshPBR mesh{ vertices, indices };
				meshes.emplace_back(mesh);
			}
		}

    }

    void ModelPBR::Draw()
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw();
    }


    void ModelPBR::processNode(aiNode* node, const aiScene* scene)
    {
        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh));
        }

        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }


    MeshPBR ModelPBR::processMesh(aiMesh* mesh)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
			SetVertexBoneDataToDefault(vertex);
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
		
		HasAnimation = ExtractBoneWeightForVertices(vertices, mesh);
        return MeshPBR(vertices, indices);
    }

	void ModelPBR::SetVertexBoneDataToDefault(Vertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.BoneIds[i] = -1;
			vertex.Weights[i] = 0.0f;
		}
	}

	void ModelPBR::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
		{
			if (vertex.BoneIds[i] < 0)
			{
				vertex.Weights[i] = weight;
				vertex.BoneIds[i] = static_cast<float>(boneID);
				break;
			}
		}
	}

	bool ModelPBR::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh)
	{
		auto& boneInfoMap = m_BoneInfoMap;
		int& boneCount = m_BoneCounter;

		if (mesh->mNumBones == 0)
			return false;

		for (unsigned int boneIndex = 0; boneIndex < static_cast<unsigned int>(mesh->mNumBones); ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				BoneInfo newBoneInfo;
				newBoneInfo.m_ID = boneCount;
				newBoneInfo.m_Offset = AssimpHelper::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
				boneInfoMap[boneName] = newBoneInfo;
				boneID = boneCount;
				boneCount++;
			}
			else
			{
				boneID = boneInfoMap[boneName].m_ID;
			}

			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}

		return true;
	}
}