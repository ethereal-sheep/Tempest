#pragma once
#include "Util.h"
#include "Animator.h"
#include "Core.h"
#include "TMath.h"
#include "Util/thread_pool.h"

namespace Tempest
{
	class AnimationManager
	{
		tmap<uint32_t, Animator> m_Animators;

	public:

		void AddAnimator(uint32_t id, Animator anim);					// Add Entity's Animator into Animation Manager
		void UpdateAnimations(float dt);								// Updates all the Animations 
		tvector<future_bool> AsyncUpdateAnimations(float dt);
		void ChangeAnimation(uint32_t id, Animation* animation);		// Changes 
		bool CheckAnimation(uint32_t id, std::string name);
		bool CheckAnimator(uint32_t id);
		void ChangeDuration(uint32_t id, float duration);

		tvector<glm::mat4> GetBoneMatrix(uint32_t id) { return m_Animators.at(id).GetFinalBoneMatrix(); }
	};
}

