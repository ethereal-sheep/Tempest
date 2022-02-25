#include "Animation.h"
#include "assimp/postprocess.h"

namespace Tempest
{
	Animation::Animation(const std::string& animationPath, AnimationModel* model)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		assert(scene && scene->mRootNode);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_Ticks = static_cast<float>(animation->mTicksPerSecond);
		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHeirarchyData(m_Root, scene->mRootNode);
		ReadMissingBones(animation, *model);
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

	void Animation::ReadMissingBones(const aiAnimation* animation, AnimationModel& model)
	{
		int size = animation->mNumChannels;

		//auto& boneInfoMap = model.GetBoneInfoMap();
		//int& boneCount = model.GetBoneCount(); 
		//
		//for (int i = 0; i < size; i++)
		//{
		//	auto channel = animation->mChannels[i];
		//	std::string boneName = channel->mNodeName.data;
		//
		//	if (boneInfoMap.find(boneName) == boneInfoMap.end())
		//	{
		//		boneInfoMap[boneName].id = boneCount;
		//		boneCount++;
		//	}
		//	m_Bones.push_back(Bone(channel->mNodeName.data,
		//		boneInfoMap[channel->mNodeName.data].id, channel));
		//}
		//
		//m_BoneInfoMap = boneInfoMap;
	}

	void Animation::ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src)
	{
		dest.m_Name = src->mName.data;
		dest.m_Transform = AssimpHelper::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.m_NumOfChildren = src->mNumChildren;

		for (int i = 0; i < src->mNumChildren; i++)
		{
			AssimpNodeData newData;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest.m_Children.push_back(newData);
		}
	}
}