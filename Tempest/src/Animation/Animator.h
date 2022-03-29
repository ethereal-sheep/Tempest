#pragma once
#include "Animation.h"
#include "Util.h"
#include "TMath.h"
#include "Core.h"

namespace Tempest
{
	class Animator
	{
	public:

		Animator(Animation* animation);
		void UpdateAnimation(float dt);
		void ChangeAnimation(Animation* animtion);
		void PlayAnimation(Animation* pAnimation);
		void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);
		tvector<glm::mat4> GetFinalBoneMatrix() { return m_BoneMatrices; }
		std::string GetName() { return m_Animation->GetName(); }
		void ChangeDuration(float duration) { m_Animation->ChangeDuration(duration); }
		//bool IsAnimationEnded()
	private:

		tvector<glm::mat4> m_BoneMatrices;
		float m_CurrentTime;
		float m_DeltaTime;
		bool m_Ended = false;
		Animation* m_Animation;
	};
}