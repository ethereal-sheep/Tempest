#pragma once
#include "Util.h"
#include <TMath.h>
#include <Core.h>
#include <functional>
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "Bone.h"
#include "Graphics/PBR/ModelPBR.h"

namespace Tempest
{
	struct AssimpNodeData
	{
		glm::mat4 m_Transform;
		std::string m_Name;
		int m_NumOfChildren;
		tvector<AssimpNodeData> m_Children;
	};

	class Animation
	{
		float m_Duration;
		float m_Ticks;
		tvector<Bone> m_Bones;
		AssimpNodeData m_Root;
		tomap<std::string, BoneInfo> m_BoneInfoMap;

	public:
		Animation() = default;
		Animation(const std::string& animationPath, ModelPBR* model);
		~Animation();

		Bone* FindBone(const std::string& name);

		inline float GetTicksPerSecond() { return m_Ticks; }
		inline float GetDuration() { return m_Duration; }
		inline const AssimpNodeData& GetRootNode() { return m_Root; }
		inline const tomap<std::string, BoneInfo>& GetBoneMap(){ return m_BoneInfoMap; }

	private:

		void ReadMissingBones(const aiAnimation* animation, ModelPBR& model);
		void ReadHeirarchyData(AssimpNodeData& dest, const aiNode* src);
	};
}