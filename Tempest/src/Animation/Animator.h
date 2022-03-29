#pragma once
#include "Animation.h"
#include "Util.h"
#include "TMath.h"
#include "Core.h"

namespace Tempest
{
	enum class ANIMATOR_STATE
	{
		PLAY,
		PAUSE,
		STOP,
		IDLE
	};

	enum class ANIMATION_TYPE
	{
		LOOP,
		PLAY_ONCE
	};

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
		bool IsAnimationEnded() { return m_Ended; }

		void Play(bool loop);
		void Pause();
		void Stop();
		void SetSpeed(float speed);

		ANIMATOR_STATE GetState() const { return m_State; }
		ANIMATION_TYPE GetType() const { return m_Type; }

		bool hasEnded() const { return m_Ended; }
	private:

		tvector<glm::mat4> m_BoneMatrices;
		float m_CurrentTime;
		float m_DeltaTime;
		bool m_Ended = false;
		float m_Speed = 1.f;
		Animation* m_Animation;

		ANIMATOR_STATE m_State = ANIMATOR_STATE::IDLE;
		ANIMATION_TYPE m_Type = ANIMATION_TYPE::PLAY_ONCE;
	};
}