#pragma once
#include "Bone.h"

namespace Tempest
{
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	class BoneAnimation
	{
	public:

		BoneAnimation(const std::string& animationPath, std::map<std::string, BoneInfo> BoneInfoMap, int count);
		~BoneAnimation();
		Bone* FindBone(const std::string& name);
		inline float GetTicksPerSecond() { return m_TicksPerSecond; }
		inline float GetDuration() { return m_Duration; }
		inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
		inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
		{
			return m_BoneInfoMap;
		}

	private:

		void ReadMissingBones(const aiAnimation* animation, std::map<std::string, BoneInfo> BoneInfoMap, int count);
		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);

		float m_Duration;
		int m_TicksPerSecond;
		std::vector<Bone> m_Bones;
		AssimpNodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
	};
}