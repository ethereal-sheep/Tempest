/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "Animation.h"
#include "assimp/postprocess.h"

namespace Tempest
{
	Animation::Animation(tomap<std::string, BoneInfo>& boneInfoMap, int& boneCount, const aiScene* scene, int index)
	{
		auto animation = scene->mAnimations[index];
		m_Name = std::string{ animation->mName.C_Str() };
		auto asd = animation->mNumChannels;
		auto qqq = animation->mChannels[asd - 1];
		auto qwe = qqq->mNumPositionKeys;
		m_Duration = static_cast<float>(qqq->mPositionKeys[qwe - 1].mTime);
		//m_Duration = static_cast<float>(animation->mDuration);
		m_Ticks = static_cast<float>(animation->mTicksPerSecond);
		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHeirarchyData(m_Root, scene->mRootNode);
		ReadMissingBones(animation, boneInfoMap, boneCount);
	}

	Animation::~Animation()
	{
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& Bone)
			{
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == m_Bones.end()) return nullptr;
		else return &(*iter);
	}

	void Animation::ReadMissingBones(const aiAnimation* animation, tomap<std::string, BoneInfo>& boneInfoMap, int& boneCount)
	{
		int size = animation->mNumChannels;
		
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;
		
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].m_ID = boneCount;
				boneCount++;
			}
			m_Bones.push_back(Bone(channel->mNodeName.data,
				boneInfoMap[channel->mNodeName.data].m_ID, channel));
		}
		
		m_BoneInfoMap = boneInfoMap;
	}

	void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
	{
		dest.m_Name = src->mName.data;
		dest.m_Transform = AssimpHelper::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.m_NumOfChildren = src->mNumChildren;

		for (unsigned int i = 0; i < static_cast<unsigned int>(src->mNumChildren); i++)
		{
			AssimpNodeData newData;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest.m_Children.push_back(newData);
		}
	}
}