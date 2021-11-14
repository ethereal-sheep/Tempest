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


    void ModelPBR::loadModel(std::string file)
    {
		std::filesystem::path p{ file };
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (strcmp(p.extension().string().c_str(), ".a"))
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
			std::vector<uint32_t> mats;
			std::vector<MaterialPBR> mm;
			MaterialPBR m;

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
						TexturePBR tex;
						tex.setTexture(p.c_str(), parent.filename().string(), true);
						m.addTexture(parent.filename().string(), std::move(tex));
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
			}
			mm.push_back(m);

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

				for (int j = joints; j < joints + sides[i]; ++j)
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
            this->meshes.push_back(this->processMesh(mesh, scene));
        }

        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }


    MeshPBR ModelPBR::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
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

        return MeshPBR(vertices, indices);
    }


}