#include "Animator.h"

namespace Tempest
{
	Animator::Animator(Animation* animation)
	{
		m_CurrentTime = 0.0f;
		m_Animation = animation;

		m_BoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_BoneMatrices.push_back(glm::mat4(1.0f));
	}

	void Animator::UpdateAnimation(float dt)
	{
		m_DeltaTime = dt;
		if (m_Animation)
		{
			m_CurrentTime += m_Animation->GetTicksPerSecond() * dt * 25.f;
			m_CurrentTime = fmod(m_CurrentTime, m_Animation->GetDuration());
			CalculateBoneTransform(&m_Animation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void Animator::ChangeAnimation(Animation* animation)
	{
		m_Animation = animation;
		m_CurrentTime = 0.f;
		m_DeltaTime = 0.f;
		m_BoneMatrices.clear();

		m_BoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_BoneMatrices.push_back(glm::mat4(1.0f));
	}

	void Animator::PlayAnimation(Animation* pAnimation)
	{
		m_Animation = pAnimation;
		m_CurrentTime = 0.0f;
	}

	void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->m_Name;
		glm::mat4 nodeTransform = node->m_Transform;

		Bone* Bone = m_Animation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_Animation->GetBoneMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].m_ID;
			glm::mat4 offset = boneInfoMap[nodeName].m_Offset;
			m_BoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->m_NumOfChildren; i++)
			CalculateBoneTransform(&node->m_Children[i], globalTransformation);
	}
}