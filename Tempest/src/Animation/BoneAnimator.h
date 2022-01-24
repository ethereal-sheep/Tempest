#pragma once
#include "BoneAnimation.h"

namespace Tempest
{
	class Animator
	{
	public:
		Animator(BoneAnimation* animation);
		void UpdateAnimation(float dt);
		void PlayAnimation(BoneAnimation* pAnimation);

		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

		std::vector<glm::mat4> GetFinalBoneMatrices();

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		BoneAnimation* m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;

	};
}