/**********************************************************************************
* \author		Cantius Chew (c.chew@digipen.edu)
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2021
* \note			Course: GAM300
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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

/*
*	Check to see if Mesh has Bones	pMesh->HasBones();
* 
*	#1 - Has Bones	[ Bone Animation ] (Pain in the ass)
*	#2 - No Bones	[ KeyFrame Animation] 
* 
*	KF -- Vertices stay the same for each key frame. different matrix for different keyframe. interpolate the matrix based on duration of each frame
*	Figure out a way to store matrices?
*/

struct Animation
{
	std::vector<aiVectorKey> positions;
	std::vector<aiQuatKey> rotations;
	std::vector<aiVectorKey> sizes;

	aiString name;
	float ticks;
	float duration;
};

struct BoneAnimation
{
	std::vector<aiVectorKey> positions;
	std::vector<aiQuatKey> rotations;
	std::vector<aiVectorKey> sizes;

	std::vector<uint32_t> numPositions;
	std::vector<uint32_t> numRotations;
	std::vector<uint32_t> numScale;

	std::vector<aiString> name;
	std::vector<float> ticks;
	std::vector<float> duration;
	std::vector<uint32_t> channels;
};

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
	std::vector<Animation> animations;

	BoneAnimation b_animations;
	std::vector<float> weights;
	std::vector<uint32_t> ids;
	std::vector<uint32_t> numWeights;

	std::vector<aiColor3D> Diffuse;
	std::vector<aiColor3D> Ambient;
	std::vector<aiColor3D> Specular;
	std::vector<aiColor3D> Emissive;
	std::vector<aiColor3D> Transparent;
	std::vector<aiColor3D> Reflective ;

	float Refraction = 0.f;
	float Reflection = 0.f;
	float Shininess = 0.f;
	float ShininessStrength = 0.f;
	float Opacity = 1.f;
};

void ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform, Mesh& mesh, uint32_t& offset);
void ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform, Mesh& m, uint32_t& offset);

void ProcessKeyFrameAnimation(std::vector<Animation>& animations);
void ProcessBoneAnimation(BoneAnimation& animations);

bool WriteBones(const Mesh& m, const std::string& path);
bool WriteToFile(const Mesh& m, const std::string& path);

void ProcessMaterials(Mesh& mMesh);

void ProcessBoneAnimation(BoneAnimation& animations)
{
	for (uint32_t i = 0; i < s_Scene->mNumAnimations; ++i)
	{
		aiAnimation* pAnim = s_Scene->mAnimations[i];

		animations.name.push_back(pAnim->mName);
		animations.ticks.push_back(pAnim->mTicksPerSecond);
		animations.duration.push_back(pAnim->mDuration);
		animations.channels.push_back(pAnim->mNumChannels);

		for (uint32_t q = 0; q < pAnim->mNumChannels; ++q)
		{
			auto* pChannel = pAnim->mChannels[q];

			animations.numPositions.push_back(pChannel->mNumPositionKeys);
			animations.numRotations.push_back(pChannel->mNumRotationKeys);
			animations.numScale.push_back(pChannel->mNumScalingKeys);

			for (auto j = 0; j < pChannel->mNumPositionKeys; ++j)
				animations.positions.push_back(pChannel->mPositionKeys[j]);

			for (auto j = 0; j < pChannel->mNumRotationKeys; ++j)
				animations.rotations.push_back(pChannel->mRotationKeys[j]);

			for (auto j = 0; j < pChannel->mNumScalingKeys; ++j)
				animations.sizes.push_back(pChannel->mScalingKeys[j]);
		}
	}
}

void ProcessNodeData(const aiNode* node, const aiMatrix4x4& transform, Mesh& mesh, uint32_t& offset)
{
	for (uint32_t i = 0; i < node->mNumMeshes; ++i)
		ProcessMeshData(s_Scene->mMeshes[node->mMeshes[i]], transform * node->mTransformation, mesh, offset);

	for (uint32_t i = 0; i < node->mNumChildren; ++i)
		ProcessNodeData(node->mChildren[i], transform * node->mTransformation, mesh, offset);
}

void ProcessMeshData(const aiMesh* mesh, const aiMatrix4x4& transform, Mesh& m, uint32_t& offset)
{
	if (mesh->HasBones())
	{
		for (uint32_t i = 0; i < mesh->mNumBones; ++i)
		{
			auto weights = mesh->mBones[i]->mNumWeights;
			m.numWeights.push_back(mesh->mBones[i]->mNumWeights);
			for (uint32_t j = 0; j < weights; ++j)
			{
				m.weights.push_back(mesh->mBones[i]->mWeights[j].mWeight);
				m.ids.push_back(mesh->mBones[i]->mWeights[j].mVertexId);
			}
		}
	}

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

		for (auto& i : m.Ambient)
		{
			std::stringstream ss;
			ss << "Ambient ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Diffuse)
		{
			std::stringstream ss;
			ss << "Diffuse ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Specular)
		{
			std::stringstream ss;
			ss << "Specular ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Emissive)
		{
			std::stringstream ss;
			ss << "Emissive ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Transparent)
		{
			std::stringstream ss;
			ss << "Transparent ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Reflective)
		{
			std::stringstream ss;

			ss << "Reflective ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.animations)
		{
			std::stringstream ss;

			ss << "aa ";
			ss << i.name.C_Str();
			ss << "\n";

			ss << "ticks ";
			ss << i.ticks;
			ss << "\n";

			ss << "duration ";
			ss << i.duration;
			ss << "\n";

			for (auto& k : i.positions)
			{
				ss << "ap ";
				ss << k.mValue.x << " ";
				ss << k.mValue.y << " ";
				ss << k.mValue.z << " ";
				ss << "\n";
			}
				
			for (auto& k : i.rotations)
			{
				ss << "ar ";
				ss << k.mValue.x << " ";
				ss << k.mValue.y << " ";
				ss << k.mValue.z << " ";
				ss << k.mValue.w << " ";
				ss << "\n";
			}

			for (auto& k : i.sizes)
			{
				ss << "as ";
				ss << k.mValue.x << " ";
				ss << k.mValue.y << " ";
				ss << k.mValue.z << " ";
				ss << "\n";
			}

			for (auto& k : i.sizes)
			{
				ss << "at ";
				ss << k.mTime;
				ss << "\n";
			}

			file << ss.str() << "\n";
		}

		std::stringstream ss;

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

bool WriteBones(const Mesh& m, const std::string& path)
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

		for (auto& i : m.Ambient)
		{
			std::stringstream ss;
			ss << "Ambient ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Diffuse)
		{
			std::stringstream ss;
			ss << "Diffuse ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Specular)
		{
			std::stringstream ss;
			ss << "Specular ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Emissive)
		{
			std::stringstream ss;
			ss << "Emissive ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Transparent)
		{
			std::stringstream ss;
			ss << "Transparent ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		for (auto& i : m.Reflective)
		{
			std::stringstream ss;

			ss << "Reflective ";
			ss << i.r << " ";
			ss << i.g << " ";
			ss << i.b;

			file << ss.str() << "\n";
		}

		std::stringstream ss;

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

		for (auto& i : m.b_animations.name)
		{
			ss << "aa ";
			ss << i.C_Str();
			ss << "\n";
		}
		
		for (auto& i : m.b_animations.ticks)
		{
			ss << "ticks ";
			ss << i;
			ss << "\n";
		}
		

		for (auto& i : m.b_animations.duration)
		{
			ss << "duration ";
			ss << i;
			ss << "\n";
		}
		

		for (auto& i : m.b_animations.positions)
		{
			ss << "ap ";
			ss << i.mValue.x << " ";
			ss << i.mValue.y << " ";
			ss << i.mValue.z << " ";
			ss << "\n";
		}

		for (auto& i : m.b_animations.rotations)
		{
			ss << "ar ";
			ss << i.mValue.x << " ";
			ss << i.mValue.y << " ";
			ss << i.mValue.z << " ";
			ss << i.mValue.w << " ";
			ss << "\n";
		}

		for (auto& i : m.b_animations.sizes)
		{
			ss << "as ";
			ss << i.mValue.x << " ";
			ss << i.mValue.y << " ";
			ss << i.mValue.z << " ";
			ss << "\n";
		}

		for (auto& i : m.b_animations.sizes)
		{
			ss << "at ";
			ss << i.mTime;
			ss << "\n";
		}

		for (auto& i : m.b_animations.numRotations)
		{
			ss << "nr ";
			ss << i;
			ss << "\n";
		}

		for (auto& i : m.b_animations.numScale)
		{
			ss << "ns ";
			ss << i;
			ss << "\n";
		}

		for (auto& i : m.b_animations.channels)
		{
			ss << "nc ";
			ss << i;
			ss << "\n";
		}

		for (auto& i : m.weights)
		{
			ss << "weights ";
			ss << i;
			ss << "\n";
		}

		for (auto& i : m.ids)
		{
			ss << "id ";
			ss << i;
			ss << "\n";
		}

		for (auto& i : m.b_animations.numPositions)
		{
			ss << "np ";
			ss << i;
			ss << "\n";
		}

		for (auto& i : m.numWeights)
		{
			ss << "nw ";
			ss << i;
			ss << "\n";
		}

		file << ss.str() << "\n";
	}

	return true;
}

void ProcessKeyFrameAnimation(std::vector<Animation>& animations)
{
	for (uint32_t i = 0; i < s_Scene->mNumAnimations; ++i)
	{
		Animation anim;
		s_Scene->mName;
		aiAnimation* pAnim = s_Scene->mAnimations[i];

		anim.name = pAnim->mName;
		anim.ticks = pAnim->mTicksPerSecond;
		anim.duration = pAnim->mDuration;

		for (uint32_t q = 0; q < pAnim->mNumChannels; ++q)
		{
			auto* pChannel = pAnim->mChannels[q];

			for (auto j = 0; j < pChannel->mNumPositionKeys; ++j)
				anim.positions.push_back(pChannel->mPositionKeys[j]);

			for (auto j = 0; j < pChannel->mNumRotationKeys; ++j)
				anim.rotations.push_back(pChannel->mRotationKeys[j]);

			for (auto j = 0; j < pChannel->mNumScalingKeys; ++j)
				anim.sizes.push_back(pChannel->mScalingKeys[j]);
		}

		animations.emplace_back(anim);
	}

}

void ProcessMaterials(Mesh& mMesh)
{
	for (uint32_t i = 0; i < s_Scene->mNumMaterials; ++i)
	{
		aiString atex;

		const auto* pMaterial = s_Scene->mMaterials[i];

		aiColor3D tDiffuse;
		aiColor3D tAmbient;
		aiColor3D tSpecular;
		aiColor3D tEmissive;
		aiColor3D tTransparent;
		aiColor3D tReflective;

		pMaterial->Get(AI_MATKEY_REFRACTI, mMesh.Refraction);
		pMaterial->Get(AI_MATKEY_REFLECTIVITY, mMesh.Reflection);
		pMaterial->Get(AI_MATKEY_SHININESS, mMesh.Shininess);
		pMaterial->Get(AI_MATKEY_SHININESS_STRENGTH, mMesh.ShininessStrength);
		pMaterial->Get(AI_MATKEY_OPACITY, mMesh.Opacity);

		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, tDiffuse);
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, tAmbient);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, tSpecular);
		pMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, tEmissive);
		pMaterial->Get(AI_MATKEY_COLOR_TRANSPARENT, tTransparent);
		pMaterial->Get(AI_MATKEY_COLOR_REFLECTIVE, tReflective);

		pMaterial->Get(AI_MATKEY_TEXTURE(aiTextureType_BASE_COLOR, 0), atex);

		if (atex.length)
		{
			std::string full_path{ atex.data };
			std::string lower_path{ atex.data };
			std::transform(lower_path.begin(), lower_path.end(), lower_path.begin(),
				[](char c) { return (char)std::tolower((int)c); });

			auto check = lower_path.find("models");
			if (check == std::string::npos) continue;

			std::string tex_path = full_path.substr(check, full_path.length());
			std::filesystem::path es{ tex_path };
			es.replace_extension(".dds");
			mMesh.textures.push_back(es.string());
		}
		else
			mMesh.textures.push_back("NULL");

		mMesh.Diffuse.push_back(tDiffuse);
		mMesh.Ambient.push_back(tAmbient);
		mMesh.Emissive.push_back(tEmissive);
		mMesh.Specular.push_back(tSpecular);
		mMesh.Reflective.push_back(tReflective);
		mMesh.Transparent.push_back(tTransparent);
	}
}

bool LoadModel(const std::string& path)
{
	s_Scene = s_Importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	if (!s_Scene)
		return false;

	if (s_Scene->mNumMeshes == 0)
		return false;

	if (s_Scene->mMeshes[0]->HasBones())		// Bone Animation
	{
		uint32_t offset = 0;
		Mesh mMesh;

		ProcessNodeData(s_Scene->mRootNode, aiMatrix4x4{}, mMesh, offset);
		ProcessBoneAnimation(mMesh.b_animations);
		ProcessMaterials(mMesh);

		if (!WriteBones(mMesh, path))
			return false;
	}
	
	else                                            // KeyFrame Animation, Normal Model Loading
	{
		uint32_t offset = 0;
		Mesh mMesh;

		ProcessNodeData(s_Scene->mRootNode, aiMatrix4x4{}, mMesh, offset);
		ProcessKeyFrameAnimation(mMesh.animations);
		ProcessMaterials(mMesh);
		
		if (!WriteToFile(mMesh, path))
			return false;
	}

	return true;
}

int main(int argc, char* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int t = argc;
	std::filesystem::path pt{ argv[1] };
	std::string ext = pt.extension().string();
	//if (strcmp(ext.c_str(), ".fbx"))
	//{
	//	//LOG("Incorrect File Type!");
	//	std::cout << "Incorrect File Type" << std::endl;
	//	return -1;
	//}

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
