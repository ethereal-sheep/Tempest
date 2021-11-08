#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glew.h>
#include <cstdlib>
#include <crtdbg.h>
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <glm.hpp>
#include <sstream>
#include <cstring>
#include "Tempest/src/Logger/Log.h"

thread_local Assimp::Importer s_Importer;
thread_local const aiScene* s_Scene;

struct Mesh
{
	std::vector<glm::vec3> pos;
	std::vector<glm::vec3> norm;
	std::vector<glm::vec2> tex;
	std::vector<glm::ivec3> indices;	
	std::vector<unsigned int> nvertices;
	std::vector<unsigned int> nfaces;
	std::vector<unsigned int> matindex;

	std::vector<std::string> textures;

	glm::vec3 Diffuse = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Ambient = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Specular = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Emissive = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Transparent = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 Reflective = glm::vec3(0.f, 0.f, 0.f);

	float Refraction = 0.f;
	float Reflection = 0.f;
	float Shininess = 0.f;
	float ShininessStrength = 0.f;
	float Opacity = 1.f;
};

void ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform, Mesh& mesh, uint32_t& offset);
void ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform, Mesh& m, uint32_t& offset);

void ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform, Mesh& mesh, uint32_t& offset)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		ProcessMeshData(s_Scene->mMeshes[node->mMeshes[i]], transform * node->mTransformation, mesh, offset);

	for (uint32_t i = 0; i < node->mNumChildren; ++i)
		ProcessNodeData(node->mChildren[i], transform * node->mTransformation, mesh, offset);
}

void ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform, Mesh& m, uint32_t& offset)
{
	// Store vertices, normals and uv
	const aiVector3D zero3D(0.f, 0.f, 0.f);
	for (size_t i = 0; i < mesh->mNumVertices; ++i)
	{
		const aiVector3D* pPos = &(mesh->mVertices[i]);
		const aiVector3D* pNormal = &(mesh->mNormals[i]);
		const aiVector3D* pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &zero3D;

		auto p = transform * (*pPos);
		m.pos.push_back(glm::vec3(p.x, p.y, p.z));
		m.norm.push_back(glm::vec3(pNormal->x, pNormal->y, pNormal->z));
		m.tex.push_back(glm::vec2(pTexCoord->x, pTexCoord->y));

		//std::cout << "Normals: " << pNormal->x << ", " << pNormal->y << ", " << pNormal->z << std::endl;
	}
	m.nvertices.push_back(mesh->mNumVertices);

	for (size_t i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		m.indices.push_back(glm::ivec3(offset + (int32_t)face.mIndices[0], offset + (int32_t)face.mIndices[1], offset + (int32_t)face.mIndices[2]));
	}
	m.nfaces.push_back(mesh->mNumFaces);
	m.matindex.push_back(mesh->mMaterialIndex);
	//offset += mesh->mNumVertices;
}

/*
*	### File Contents ###
* 
*	[v] - Vertices(Positions)
*	[n] - Normals
*	[t] - Texture Coordinates
*	[f] - Indices(Faces)
*	[p] - Texture Path
*/
bool WriteToFile(const Mesh& m, const std::string& path)
{
	std::filesystem::path p{ path };
	auto name = p.replace_extension(".a");
	std::ofstream file{ name.string() };
	if (!file.is_open())
	{
		std::cout << "Failed to Open File" << std::endl;
		return false;
	}

	else
	{
		for (auto& i : m.pos)
		{
			std::stringstream ss;
			ss << "v ";
			ss << i.x << " ";
			ss << i.y << " ";
			ss << i.z;

			file << ss.str() << "\n";
		}

		for (auto& i : m.norm)
		{
			std::stringstream ss;
			ss << "n ";
			ss << i.x << " ";
			ss << i.y << " ";
			ss << i.z;

			file << ss.str() << "\n";
		}

		for (auto& i : m.tex)
		{
			std::stringstream ss;
			ss << "t ";
			ss << i.x << " ";
			ss << i.y;

			file << ss.str() << "\n";
		}

		for (auto& i : m.indices)
		{
			std::stringstream ss;
			ss << "f ";
			ss << i.x << " ";
			ss << i.y << " ";
			ss << i.z;

			file << ss.str() << "\n";
		}

		for (auto& i : m.textures)
		{
			std::stringstream ss;
			ss << "p ";
			ss << i;

			file << ss.str() << "\n";
		}

		for (auto& i : m.nvertices)
		{
			std::stringstream ss;
			ss << "m ";
			ss << i;

			file << ss.str() << "\n";
		}

		for (auto& i : m.nfaces)
		{
			std::stringstream ss;
			ss << "w ";
			ss << i;

			file << ss.str() << "\n";
		}

		for (auto& i : m.matindex)
		{
			std::stringstream ss;
			ss << "g ";
			ss << i;

			file << ss.str() << "\n";
		}

		std::stringstream ss;
		ss << "Ambient ";
		ss << m.Ambient.x << " ";
		ss << m.Ambient.y << " ";
		ss << m.Ambient.z;
		
		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Diffuse ";
		ss << m.Diffuse.x << " ";
		ss << m.Diffuse.y << " ";
		ss << m.Diffuse.z;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Specular ";
		ss << m.Specular.x << " ";
		ss << m.Specular.y << " ";
		ss << m.Specular.z;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Emissive ";
		ss << m.Emissive.x << " ";
		ss << m.Emissive.y << " ";
		ss << m.Emissive.z;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Transparent ";
		ss << m.Transparent.x << " ";
		ss << m.Transparent.y << " ";
		ss << m.Transparent.z;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Reflective ";
		ss << m.Reflective.x << " ";
		ss << m.Reflective.y << " ";
		ss << m.Reflective.z;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Refraction ";
		ss << m.Refraction;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Reflection ";
		ss << m.Reflection;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Shininess ";
		ss << m.Shininess;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "ShininessStrength ";
		ss << m.ShininessStrength;

		file << ss.str() << "\n";

		ss.str(std::string());

		ss << "Opacity ";
		ss << m.Opacity;

		file << ss.str() << "\n";

		ss.str(std::string());
	}
	
	return true;
}

bool LoadModel(const std::string& path)
{
	s_Scene = s_Importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	if (!s_Scene)	
		return false;

	uint32_t offset = 0;
	Mesh mMesh;
	ProcessNodeData(s_Scene->mRootNode, aiMatrix4x4{}, mMesh, offset);

	for (uint32_t i = 0; i < s_Scene->mNumMaterials; ++i)
	{
		aiString atex;

		const auto* pMaterial = s_Scene->mMaterials[i];

		pMaterial->Get(AI_MATKEY_REFRACTI, mMesh.Refraction);
		pMaterial->Get(AI_MATKEY_REFLECTIVITY, mMesh.Reflection);
		pMaterial->Get(AI_MATKEY_SHININESS, mMesh.Shininess);
		pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, mMesh.ShininessStrength);
		pMaterial->Get(AI_MATKEY_OPACITY, mMesh.Opacity);

		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, &mMesh.Diffuse, nullptr);
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, &mMesh.Ambient, nullptr);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, &mMesh.Specular, nullptr);
		pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, &mMesh.Emissive, nullptr);
		pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, &mMesh.Transparent, nullptr);
		pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, &mMesh.Reflective, nullptr);

		pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, i), atex);
		if (atex.length)
		{
			std::string full_path{ atex.data };
			std::string lower_path{ atex.data };
			std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(),
				[](char c) { return (char)std::tolower((int)c); });

			auto check = lower_path.find("models");
			if (check == std::string::npos) continue;

			std::string tex_path = full_path.substr(check, full_path.length());
			mMesh.textures.push_back(tex_path);
		}
	}

	if (!WriteToFile(mMesh, path))
		return false;

	return true;
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int t = argc;
	std::filesystem::path pt{ argv[1] };
	std::string ext = pt.extension().string();
	if (strcmp(ext.c_str(), ".fbx"))
	{
		//LOG("Incorrect File Type!");
		std::cout << "Incorrect File Type" << std::endl;
		return -1;
	}

	while (t > 1)
	{	
		if (!LoadModel(argv[1]))
		{
			std::cout << "Model Failed to Load : " "[" << argv[1] << "]" << std::endl;
			return -1;
		}

		else
		{
			std::cout << "Model Loaded Successfully : " << "[" << argv[1] << "]" << std::endl;
			return 0;
		}
	}
	//LOG_WARN("No File Specified!");
	std::cout << "No File Specified!" << std::endl;
	return -1;
}
