/**********************************************************************************
* \author		Linus Ng Hao Xuan (haoxuanlinus.ng@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
		if (m_State == ANIMATOR_STATE::PLAY)
		{
			switch (m_Type)
			{
				case ANIMATION_TYPE::LOOP:
				{
					m_CurrentTime += m_Animation->GetTicksPerSecond() * dt * 25.f * m_Speed;
					m_CurrentTime = fmod(m_CurrentTime, m_Animation->GetDuration());
					CalculateBoneTransform(&m_Animation->GetRootNode(), glm::mat4(1.0f));

					break;
				}

				case ANIMATION_TYPE::PLAY_ONCE:
				{
					m_CurrentTime += m_Animation->GetTicksPerSecond() * dt * 25.f * m_Speed;
					m_Ended = m_CurrentTime > m_Animation->GetDuration();
					if (m_Ended)
						return;
					else
					{
						m_CurrentTime = fmod(m_CurrentTime, m_Animation->GetDuration());
						CalculateBoneTransform(&m_Animation->GetRootNode(), glm::mat4(1.0f));
					}
					
					break;
				}
			}

		}
	}

	void Animator::ChangeAnimation(Animation* animation)
	{
		//if(animation->GetName() == m_Animation->GetName())
		//	return;

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

	void Animator::Play(bool loop)
	{
		m_State = ANIMATOR_STATE::PLAY;
		m_Type = (loop) ? ANIMATION_TYPE::LOOP : ANIMATION_TYPE::PLAY_ONCE;
	}

	void Animator::Pause()
	{
		// Set to Pause state - no update
		m_State = ANIMATOR_STATE::PAUSE;
	}

	void Animator::Stop()
	{
		m_State = ANIMATOR_STATE::STOP;

		// Reset Animation Info

		m_CurrentTime = 0.f;
		m_DeltaTime = 0.f;
		m_BoneMatrices.clear();
		m_Speed = 1.f;
		m_Type = ANIMATION_TYPE::PLAY_ONCE;

		m_BoneMatrices.reserve(100);
		for (int i = 0; i < 100; i++)
			m_BoneMatrices.push_back(glm::mat4(1.0f));
	}

	void Animator::SetSpeed(float speed)
	{
		m_Speed = speed;
	}
}